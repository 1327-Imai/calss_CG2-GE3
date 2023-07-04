#pragma once
#include "Model.h"
#include "DirectXCommon.h"

class ModelManager{
	//エイリアス
private:
	using string = std::string;

	//静的メンバ関数
public:
	//インスタンス取得
	static ModelManager* GetInstance();

	//インスタンス生成
	static ModelManager* Create(DirectXCommon* dxCommon);

	//メンバ関数
public:
	//終了時処理
	void Finalize();

	//モデル読み込み
	void LoadModel(const string& modelName);

	//モデル呼び出し
	Model* CallModel(const string& modelName);

	//モデル削除
	void deleteModels(const string& modelName);

private:
	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//メンバ変数
private:
	//モデル配列
	std::vector<Model*> models;

protected:
	//コンストラクタ
	ModelManager();

};

