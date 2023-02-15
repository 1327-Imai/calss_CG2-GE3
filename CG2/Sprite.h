#pragma once
#include "SpriteCommon.h"

class Sprite {
public:

	//�R���X�g���N�^
	Sprite();

	//�f�X�g���N�^
	~Sprite();

	//�����o�֐�
	void Initialize(SpriteCommon spriteCommon);

	void Update();

	void Draw();

	//�\����
private:
	struct Vertex {
		Vector3 pos;	//xyz���W
		Vector3 normal;	//�@���x�N�g��
		Vector2 uv;		//uv���W
	};

	//�����o�ϐ�
private:
	//���W
	

	// ���_�f�[�^�z��
	std::vector<Vertex> vertices_;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices_;

	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB_;

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB_;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc_;

	//���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	//�C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;

	//GPU��̃o�b�t�@�̃}�b�v
	Vertex* vertMap_ = nullptr;

	//�C���f�b�N�X�o�b�t�@�̃}�b�v
	uint16_t* indexMap_ = nullptr;

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_;
};

