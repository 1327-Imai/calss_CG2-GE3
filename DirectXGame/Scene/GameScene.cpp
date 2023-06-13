#include "GameScene.h"

//コンストラクタ
GameScene::GameScene() {

}

//デストラクタ
GameScene::~GameScene() {
	delete input_;
	delete sprite_;
	delete spriteCommon_;
}

//初期化処理
void GameScene::Initialize(WinApp* winApp , DirectXCommon* dxCommon) {
	//スプライト共通処理
	spriteCommon_ = new SpriteCommon;
	spriteCommon_->Initialize(dxCommon);

	// キーボードデバイスの生成
	input_ = new Input();
	input_->Initialize(winApp);

	//ゲームループ前初期化処理
	spriteCommon_->LoadTexture(0 , "texture.jpg");
	spriteCommon_->LoadTexture(1 , "reimu.png");

	//スプライト
	sprite_ = new Sprite;
	sprite_->Initialize(spriteCommon_ , 1);
}

//更新処理
void GameScene::Update() {
	//DirectX毎フレーム処理
	//更新処理
	input_->Update();

}

//描画処理
void GameScene::Draw() {
	//スプライト描画
	spriteCommon_->PreDraw();

	sprite_->Draw();
}