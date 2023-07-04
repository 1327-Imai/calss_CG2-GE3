#pragma once
#include "Input.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3D.h"
#include "ViewProjection.h"
#include "ModelManager.h"
#include "TextureManager.h"

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
	ModelManager* modelManager_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	Sprite* sprite_ = nullptr;

	Object3D* object_ = nullptr;

	Vector3 rotation;

};

