#include "Sprite.h"
//コンストラクタ
Sprite::Sprite() {

}

//デストラクタ
Sprite::~Sprite() {

}

//メンバ関数
void Sprite::Initialize(SpriteCommon* spriteCommon) {

	//DirectX基礎
	dx12base_ = spriteCommon->GetDX12Base();

	// 頂点バッファビュー
	vbView_ = spriteCommon->GetVBView();

	verticesValue = spriteCommon->GetVerticesValue();

}

void Sprite::Update() {

}

void Sprite::Draw() {
	////頂点バッファの設定
	dx12base_->GetCmdList()->IASetVertexBuffers(0 , 1 , &vbView_);

	////描画コマンド
	dx12base_->GetCmdList()->DrawInstanced(verticesValue , 1 , 0 , 0);

}