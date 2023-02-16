#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"
#include "Input.h"

class Sprite {
public:

	//コンストラクタ
	Sprite();

	//デストラクタ
	~Sprite();

	//メンバ関数
	void Initialize(SpriteCommon* spriteCommon,Input* input);

	void Update();

	void Draw();

private:
	void CreateConstMapTransform();

	void CreateConstMapMaterial();

	//構造体
private:
	struct ConstBufferDataMaterial {
		Vector4 color;
	};

	struct ConstBufferDataTransform {
		Matrix4 mat;
	};

	//メンバ変数
private:

	Input* input_ = nullptr;

	//DirectX基礎
	DX12base* dx12base_ = nullptr;

	//頂点数
	UINT verticesValue;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//定数バッファ
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	//定数バッファビュー
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;

	ConstBufferDataTransform* constMapTransform_ = nullptr;

	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//SRVヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	UINT incremantSize_;

	Matrix4 mat;

};

