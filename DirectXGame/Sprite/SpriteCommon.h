#pragma once
#include "MathFunc.h"
#include "vector"
#include <array>
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

	D3D12_VERTEX_BUFFER_VIEW* GetVBView();

	DX12base* GetDX12Base();

	void Set2DCoordinateSystem(Matrix4& mat);

	void LoadTexture(uint32_t index , const std::string& fileName);

	void SetTextureCommands(uint32_t index);

	ComPtr<ID3D12Resource> GetTextureBuffer(uint32_t index)const;

	ComPtr<ID3D12Resource> GetVertBuff()const;

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

	//リソース設定
	D3D12_RESOURCE_DESC resDesc_{};

	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;

	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

	//SRVヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

	//ComPtr<ID3D12Resource> texBuff_ = nullptr;

	//ComPtr<ID3D12Resource> texBuff2_ = nullptr;

	//SRVの最大個数
	static const size_t kMaxSRVCount = 2056;
	std::array<ComPtr<ID3D12Resource> , kMaxSRVCount> textureBuffers_;

	static std::string kDefaultTextureDerectoryPath;

	UINT incremantSize_;

};