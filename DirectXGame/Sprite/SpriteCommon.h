#pragma once
#include "MathFunc.h"
#include "vector"
#include <array>
#include "DirectXCommon.h"

class SpriteCommon {
public:

	//コンストラクタ
	SpriteCommon();

	//デストラクタ
	~SpriteCommon();

	//メンバ関数
	void Initialize(DirectXCommon* dxCommon);

	void PreDraw();

	void Set2DCoordinateSystem(Matrix4& mat);

	UINT GetVerticesValue() {
		return _countof(vertices_);
	}

	D3D12_VERTEX_BUFFER_VIEW* GetVBView() {
		return &vbView_;
	}

	DirectXCommon* GetDxCommon() {
		return dxCommon_;
	}

	ComPtr<ID3D12Resource> GetVertBuff() const {
		return vertBuff_;
	}

	D3D12_RESOURCE_DESC GetResDesc() {
		return resDesc_;
	}

	//構造体
private:
	struct Vertex {
		Vector3 pos;	//xyz座標
		Vector2 uv;		//uv座標
	};

	//メンバ変数
private:

	//DirectX基礎
	DirectXCommon* dxCommon_ = nullptr;

	//頂点
	Vertex vertices_[4];

	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//リソース設定
	D3D12_RESOURCE_DESC resDesc_{};

	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;

	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

};