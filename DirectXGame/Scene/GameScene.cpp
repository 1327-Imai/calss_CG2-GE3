#include "GameScene.h"

//コンストラクタ
GameScene::GameScene() {

}

//デストラクタ
GameScene::~GameScene() {
	delete input_;
	delete sprite_;
	delete spriteCommon_;
	delete camera_;
	delete object_;
	modelManager_->Finalize();
	textureManager_->Finalize();
}

//初期化処理
void GameScene::Initialize(WinApp* winApp , DirectXCommon* dxCommon) {
	//スプライト共通処理
	spriteCommon_ = new SpriteCommon;
	spriteCommon_->Initialize(dxCommon);

	//モデルマネージャー
	modelManager_ = ModelManager::Create(dxCommon);
	
	//テクスチャマネージャー
	textureManager_ = TextureManager::Create(dxCommon);

	//カメラ初期化
	camera_ = new Camera;
	camera_->Initialize();

	//3Dオブジェクト共通処理
	Object3D::StaticInitialize(dxCommon,camera_);

	// キーボードデバイスの生成
	input_ = new Input();
	input_->Initialize(winApp);

	//モデル読み込み
	modelManager_->LoadModel("cube");

	//テクスチャ読み込み
	textureManager_->LoadTexture("texture.jpg");
	textureManager_->LoadTexture("white1x1.png");
	textureManager_->LoadTexture("reimu.png");
	textureManager_->LoadTexture("cube.jpg");


	//スプライト初期化
	sprite_ = new Sprite;
	sprite_->Initialize(spriteCommon_ , textureManager_->CallTexture("texture"));

	//3Dモデル初期化
	object_ = new Object3D;
	object_->Initialize();
	object_->SetModel(modelManager_->CallModel("cube"));
	object_->SetTexture(textureManager_->CallTexture("white1x1"));

	pos = {0 , 0 , -10};
	rotation = {0 , 0 , 0};

}

//更新処理
void GameScene::Update() {
	//DirectX毎フレーム処理
	//更新処理
	input_->Update();


	if (input_->PushKey(DIK_D)) {
		pos.x += MathFunc::Utility::Deg2Rad(5);
	}
	if (input_->PushKey(DIK_A)) {
		pos.x -= MathFunc::Utility::Deg2Rad(5);
	}
	if (input_->PushKey(DIK_W)) {
		pos.y += MathFunc::Utility::Deg2Rad(5);
	}
	if (input_->PushKey(DIK_S)) {
		pos.y -= MathFunc::Utility::Deg2Rad(5);
	}

	if (input_->PushKey(DIK_UP)) {
		rotation.x += MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_DOWN)) {
		rotation.x -= MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_LEFT)) {
		rotation.y -= MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_RIGHT)) {
		rotation.y += MathFunc::Utility::Deg2Rad(30);
	}
	if (input_->PushKey(DIK_X)) {
		rotation.z += MathFunc::Utility::Deg2Rad(30);
	}

	camera_->SetPosition(pos);
	camera_->SetRotation(rotation);

	camera_->Update();

	if (input_->PushKey(DIK_SPACE)) {
		object_->SetTexture(textureManager_->CallTexture("cube"));
		sprite_->SetTexture(textureManager_->CallTexture("reimu"));
	}
	else {
		object_->SetTexture(textureManager_->CallTexture("white1x1"));
		sprite_->SetTexture(textureManager_->CallTexture("texture"));
	}



	object_->Update();
	sprite_->Update();

}

//描画処理
void GameScene::Draw() {
	//3D描画
	object_->Draw();
	
	//スプライト描画
	spriteCommon_->PreDraw();

	sprite_->Draw();
}