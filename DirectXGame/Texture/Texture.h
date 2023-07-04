#pragma once
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "Model.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class Texture {
private:
	using string = std::string;

public:
	//�R���X�g���N�^
	Texture();

	//�f�X�g���N�^
	~Texture();

	//�ÓI�����o�֐�
	//�A�N�Z�b�T
	static void SetDevice(ID3D12Device* device) {
		Texture::device = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Texture::cmdList = cmdList;
	}

	//�����o�֐�
public:
	void LoadTexture(const string& fileName);

	//void CreateSRV(Model* model);

	void SetSRV(ComPtr<ID3D12DescriptorHeap>& srvHeap, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle, D3D12_RESOURCE_DESC resDesc);

	void Draw(ComPtr<ID3D12DescriptorHeap> srvHeap);

	//�A�N�Z�b�T
	string GetTextureName() {
		return textureName;
	}

	D3D12_RESOURCE_DESC GetResDesc() {
		return textureResouceDesc;
	}

private:
	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;

	//�e�N�X�`���i�[���[�g�p�X
	static const string baseDirectory;

	//�����o�ϐ�
private:
	//�摜�C���[�W�f�[�^�̍쐬
	TexMetadata metadata;
	ScratchImage scratchImg;

	//�~�b�v�}�b�v
	ScratchImage mipChain;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResouceDesc;

	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff = nullptr;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	string textureName;

};