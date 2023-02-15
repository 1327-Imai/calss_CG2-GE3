#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"

class Sprite {
public:

	//�R���X�g���N�^
	Sprite();

	//�f�X�g���N�^
	~Sprite();

	//�����o�֐�
	void Initialize(SpriteCommon* spriteCommon);

	void Update();

	void Draw();

	//�\����
private:


	//�����o�ϐ�
private:
	//DirectX��b
	DX12base* dx12base_ = nullptr;

	//���_��
	UINT verticesValue;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//���[���h�ϊ�
	WorldTransform worldTransform;

};

