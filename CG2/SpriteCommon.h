#pragma once
#include "MathFunc.h"
#include "vector"
#include "DX12base.h"

class SpriteCommon {
public:

	//コンストラクタ
	SpriteCommon();

	//デストラクタ
	~SpriteCommon();

	//メンバ関数
	void Initialize(DX12base* dx12base);

	void PreDraw();

	UINT GetVerticesValue();

	D3D12_VERTEX_BUFFER_VIEW GetVBView();

	DX12base* GetDX12Base();

	//構造体
private:
	struct Vertex {
		Vector3 pos;	//xyz座標
	};

	//メンバ変数
private:

	//DirectX基礎
	DX12base* dx12base_ = nullptr;

	//頂点
	XMFLOAT3 vertices_[3];

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;

	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
};

