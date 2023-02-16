#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"
#include "Input.h"

class Sprite {
public:

	//�R���X�g���N�^
	Sprite();

	//�f�X�g���N�^
	~Sprite();

	//�����o�֐�
	void Initialize(SpriteCommon* spriteCommon,Input* input);

	void Update();

	void Draw();

private:
	void CreateConstMapTransform();

	void CreateConstMapMaterial();

	//�\����
private:
	struct ConstBufferDataMaterial {
		Vector4 color;
	};

	struct ConstBufferDataTransform {
		Matrix4 mat;
	};

	//�����o�ϐ�
private:

	Input* input_ = nullptr;

	//DirectX��b
	DX12base* dx12base_ = nullptr;

	//���_��
	UINT verticesValue;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//�萔�o�b�t�@
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	//�萔�o�b�t�@�r���[
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;

	ConstBufferDataTransform* constMapTransform_ = nullptr;

	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//SRV�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	UINT incremantSize_;

	Matrix4 mat;

};

