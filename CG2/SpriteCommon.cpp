#include "SpriteCommon.h"

//�R���X�g���N�^
SpriteCommon::SpriteCommon() {

}

//�f�X�g���N�^
SpriteCommon::~SpriteCommon() {

}

//�����o�֐�
void SpriteCommon::Initialize(DX12base dx12base) {

	HRESULT result;

	vertices_ = {
		{{-5.0f , -5.0f , -5.0f} , {} , {0.0f , 1.0f}} ,//���� �C���f�b�N�X0
		{{-5.0f , 5.0f , -5.0f} , {} , {0.0f , 0.0f}} ,//���� �C���f�b�N�X1
		{{5.0f , -5.0f , -5.0f} , {} , {1.0f , 1.0f}} ,//�E�� �C���f�b�N�X2
		{{5.0f , 5.0f , -5.0f} , {} , {1.0f , 0.0f}} //�E�� �C���f�b�N�X3
	};

	//�C���f�b�N�X�f�[�^
	indices_ = {
		0 , 1 , 2 , //�O�p�`1��
		1 , 3 , 2 , //�O�p�`2��
	};

	//�@���̌v�Z
	for (int i = 0; i < indices_.size() / 3; i++) {

		//�O�p�`�̃C���f�b�N�X�����o���Ĉꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices_[i * 3 + 0];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		Vector3 p0 = vertices_[index0].pos;
		Vector3 p1 = vertices_[index1].pos;
		Vector3 p2 = vertices_[index2].pos;

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;

		//�O�ς͗������琂���ȃx�N�g��
		Vector3 normal = v1.cross(v2);

		//���K��
		normal.nomalize();

		//���߂��@���𒸓_�f�[�^�ɑ��
		vertices_[index0].normal = normal;
		vertices_[index1].normal = normal;
		vertices_[index2].normal = normal;

	}


	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB_ = static_cast<UINT>(sizeof(Vertex) * vertices_.size());

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB_ = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB_; //���_�f�[�^�S�̂̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&resDesc_ ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff_)
	);
	assert(SUCCEEDED(result));

	//�C���f�b�N�X�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&resDesc_ ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&indexBuff_)
	);
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff_->Map(0 , nullptr , (void**)&vertMap_);
	//�S���_�ɑ΂���
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i];	//���W���R�s�[
	}
	//�q���������
	vertBuff_->Unmap(0 , nullptr);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	result = indexBuff_->Map(0 , nullptr , (void**)&indexMap_);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < indices_.size(); i++) {
		indexMap_[i] = indices_[i];	//���W���R�s�[
	}
	//�q���������
	indexBuff_->Unmap(0 , nullptr);

	//GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB_;
	//���_�P���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB_;

}

void SpriteCommon::Update() {

}

void SpriteCommon::Draw() {

}