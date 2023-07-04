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
	//コンストラクタ
	GameScene();

	//デストラクタ
	~GameScene();

public:
	//メンバ関数
	//初期化処理
	void Initialize(WinApp* winApp,DirectXCommon* dxCommon);

	//更新処理
	void Update();

	//描画処理
	void Draw();

private:
	//メンバ変数
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

