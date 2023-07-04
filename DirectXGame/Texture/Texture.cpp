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
		//WICテクスチャのロード
		result = LoadFromWICFile(
			wfilePath.data() ,
			WIC_FLAGS_NONE ,
			&metadata ,
			scratchImg
		);
	}
	else {
		//WICテクスチャのロード
		result = LoadFromWICFile(
			L"Resources/white1x1.png" ,
			WIC_FLAGS_NONE ,
			&metadata ,
			scratchImg
		);
	}

	//ミップマップ生成
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
	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;	//幅
	textureResouceDesc.Height = (UINT)metadata.height;	//高さ
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	//テクスチャバッファにデータ転送
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i , 0 , 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//全領域へコピー
			img->pixels ,			//元データアドレス
			(UINT)img->rowPitch ,	//1ラインサイズ
			(UINT)img->slicePitch	//1枚サイズ
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

	//デスクリプタヒープの生成
	//デスクリプタヒープの設定
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(
		&srvHeapDesc ,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));

	//デスクリプタハンドル
	//SRVヒープの先頭ハンドルを取得
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダーリソースビューの作成
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff.Get() , &srvDesc , srvHandle);
}


void Texture::Draw(ComPtr<ID3D12DescriptorHeap> srvHeap) {
	//SRVヒープの設定コマンド
	cmdList->SetDescriptorHeaps(1 , srvHeap.GetAddressOf());

	//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);
}