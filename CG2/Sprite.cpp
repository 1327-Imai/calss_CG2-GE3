#include "Sprite.h"
//�R���X�g���N�^
Sprite::Sprite() {

}

//�f�X�g���N�^
Sprite::~Sprite() {

}

//�����o�֐�
void Sprite::Initialize(SpriteCommon* spriteCommon) {

	//DirectX��b
	dx12base_ = spriteCommon->GetDX12Base();

	// ���_�o�b�t�@�r���[
	vbView_ = spriteCommon->GetVBView();

	verticesValue = spriteCommon->GetVerticesValue();

}

void Sprite::Update() {

}

void Sprite::Draw() {
	////���_�o�b�t�@�̐ݒ�
	dx12base_->GetCmdList()->IASetVertexBuffers(0 , 1 , &vbView_);

	////�`��R�}���h
	dx12base_->GetCmdList()->DrawInstanced(verticesValue , 1 , 0 , 0);

}