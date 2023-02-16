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

	ComPtr<ID3D12DescriptorHeap> GetSrvHeap();

	UINT GetIncemantSize();

	void Set2DCoordinateSystem(Matrix4& mat);

	//構造体
private:
	struct Vertex {
		Vector3 pos;	//xyz座標
		Vector2 uv;		//uv座標
	};

	//メンバ変数
private:

	//DirectX基礎
	DX12base* dx12base_ = nullptr;

	//頂点
	Vertex vertices_[4];

	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;

	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

	//SRVヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	ComPtr<ID3D12Resource> texBuff_ = nullptr;

	ComPtr<ID3D12Resource> texBuff2_ = nullptr;

	UINT incremantSize_;

	Vector2 position_;




};