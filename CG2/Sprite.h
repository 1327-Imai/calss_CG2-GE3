#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"

class Sprite {
public:

	//コンストラクタ
	Sprite();

	//デストラクタ
	~Sprite();

	//メンバ関数
	void Initialize(SpriteCommon* spriteCommon);

	void Update();

	void Draw();

	//構造体
private:


	//メンバ変数
private:
	//DirectX基礎
	DX12base* dx12base_ = nullptr;

	//頂点数
	UINT verticesValue;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//ワールド変換
	WorldTransform worldTransform;

};

