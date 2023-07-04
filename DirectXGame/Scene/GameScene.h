#pragma once
#include "Input.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3D.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Camera.h"

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

	Camera* camera_ = nullptr;

	Sprite* sprite_ = nullptr;

	Object3D* object_ = nullptr;

	Vector3 pos;
	Vector3 rotation;

};

