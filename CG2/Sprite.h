#pragma once
#include "SpriteCommon.h"

class Sprite {
public:

	//コンストラクタ
	Sprite();

	//デストラクタ
	~Sprite();

	//メンバ関数
	void Initialize(SpriteCommon spriteCommon);

	void Update();

	void Draw();

	//構造体
private:
	struct Vertex {
		Vector3 pos;	//xyz座標
		Vector3 normal;	//法線ベクトル
		Vector2 uv;		//uv座標
	};

	//メンバ変数
private:
	//座標
	

	// 頂点データ配列
	std::vector<Vertex> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;

	//頂点データ全体のサイズ
	UINT sizeVB_;

	//インデックスデータ全体のサイズ
	UINT sizeIB_;

	//リソース設定
	D3D12_RESOURCE_DESC resDesc_;

	//頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	//インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;

	//GPU上のバッファのマップ
	Vertex* vertMap_ = nullptr;

	//インデックスバッファのマップ
	uint16_t* indexMap_ = nullptr;

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_;
};

