#include "Texture.h"
ID3D12Device* Texture::device = nullptr;
ID3D12GraphicsCommandList* Texture::cmdList = nullptr;

const std::string Texture::baseDirectory = "Resources/Texture/";

Texture::Texture(){
}

Texture::~Texture(){
}

void Texture::LoadTexture(const string& fileName) {

	HRESULT result;

	std::string fullPath = baseDirectory + fileName;

	int filePathBufferSize = MultiByteToWideChar(
		CP_ACP , 0 , fullPath.c_str() , -1 , nullptr , 0
	);

	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(
		CP_ACP , 0 , fullPath.c_str() , -1 , wfilePath.data() , filePathBufferSize
	);

	if (wfilePath.data()) {
		//WIC�e�N�X�`���̃��[�h
		result = LoadFromWICFile(
			wfilePath.data() ,
			WIC_FLAGS_NONE ,
			&metadata ,
			scratchImg
		);
	}
	else {
		//WIC�e�N�X�`���̃��[�h
		result = LoadFromWICFile(
			L"Resources/white1x1.png" ,
			WIC_FLAGS_NONE ,
			&metadata ,
			scratchImg
		);
	}

	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages() ,
		scratchImg.GetImageCount() ,
		scratchImg.GetMetadata() ,
		TEX_FILTER_DEFAULT ,
		0 ,
		mipChain
	);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;	//��
	textureResouceDesc.Height = (UINT)metadata.height;	//����
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&textureHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i , 0 , 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//�S�̈�փR�s�[
			img->pixels ,			//���f�[�^�A�h���X
			(UINT)img->rowPitch ,	//1���C���T�C�Y
			(UINT)img->slicePitch	//1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}


	textureName = fileName;
	for (int i = 0; i < 4; i++) {
		textureName.pop_back();
	}
}

void Texture::SetSRV(ComPtr<ID3D12DescriptorHeap>& srvHeap , D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle , D3D12_RESOURCE_DESC resDesc){
	HRESULT result;

	//�f�X�N���v�^�q�[�v�̐���
	//�f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(
		&srvHeapDesc ,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�n���h��
	//SRV�q�[�v�̐擪�n���h�����擾
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�V�F�[�_�[���\�[�X�r���[�̍쐬
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff.Get() , &srvDesc , srvHandle);
}


void Texture::Draw(ComPtr<ID3D12DescriptorHeap> srvHeap) {
	//SRV�q�[�v�̐ݒ�R�}���h
	cmdList->SetDescriptorHeaps(1 , srvHeap.GetAddressOf());

	//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);
}