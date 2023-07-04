#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include <fstream>
#include <sstream>
#include <string>

class Model
{
public:
	//コンストラクタ
	Model();

	//デストラクタ
	~Model();
private://エイリアス
	//stdを省略
	using string = std::string;

public://定数
	//モデル格納ルートパス
	static const string baseDirectory;

public:
	//静的メンバ関数
	//アクセッサ
	static void SetDevice(ID3D12Device* device) {
		Model::device = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Model::cmdList = cmdList;
	}

	//メンバ関数
public:
	//初期化
	void LoadModel(const string& modelName);

	//描画
	void Draw();

	//モデル名取得
	string GetModelName() {
		return modelName;
	}

	D3D12_RESOURCE_DESC GetResDesc() {
		return resDesc;
	}

	//構造体
private:
	struct VertexPosNormalUv {
		Vector3 pos;	//xyz座標
		Vector3 normal;	//法線ベクトル
		Vector2 uv;	//uv座標
	};

	//静的メンバ変数
private:
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;

	//メンバ変数
private:
	//頂点
	std::vector<VertexPosNormalUv> vertices;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//インデックス
	std::vector<uint16_t> indices;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	//リソース設定
	D3D12_RESOURCE_DESC resDesc;

	//モデル名
	string modelName;

};

