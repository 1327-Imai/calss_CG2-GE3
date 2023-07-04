#pragma once
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "Model.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class Texture {
private:
	using string = std::string;

public:
	//コンストラクタ
	Texture();

	//デストラクタ
	~Texture();

	//静的メンバ関数
	//アクセッサ
	static void SetDevice(ID3D12Device* device) {
		Texture::device = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Texture::cmdList = cmdList;
	}

	//メンバ関数
public:
	void LoadTexture(const string& fileName);

	//void CreateSRV(Model* model);

	void SetSRV(ComPtr<ID3D12DescriptorHeap>& srvHeap, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle, D3D12_RESOURCE_DESC resDesc);

	void Draw(ComPtr<ID3D12DescriptorHeap> srvHeap);

	//アクセッサ
	string GetTextureName() {
		return textureName;
	}

	D3D12_RESOURCE_DESC GetResDesc() {
		return textureResouceDesc;
	}

private:
	//静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;

	//テクスチャ格納ルートパス
	static const string baseDirectory;

	//メンバ変数
private:
	//画像イメージデータの作成
	TexMetadata metadata;
	ScratchImage scratchImg;

	//ミップマップ
	ScratchImage mipChain;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp;

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc;

	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff = nullptr;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	string textureName;

};