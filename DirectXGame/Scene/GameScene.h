#pragma once
#include "Input.h"
#include "SpriteCommon.h"
#include "Sprite.h"

class GameScene{
public:
	//�R���X�g���N�^
	GameScene();

	//�f�X�g���N�^
	~GameScene();

public:
	//�����o�֐�
	//����������
	void Initialize(WinApp* winApp,DirectXCommon* dxCommon);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

private:
	//�����o�ϐ�
	Input* input_ = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;

	Sprite* sprite_ = nullptr;

};

