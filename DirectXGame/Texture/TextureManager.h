#pragma once
#include "Texture.h"
#include "DirectXCommon.h"

class TextureManager{
	//エイリアス
private:
	using string = std::string;

	//静的メンバ関数
public:
	//インスタンス取得
	static TextureManager* GetInstance();

	//インスタンス生成
	static TextureManager* Create(DirectXCommon* dxCommon);

	//メンバ関数
public:
	//終了時処理
	void Finalize();

	//テクスチャ読み込み
	void LoadTexture(const string& textureName);

	//テクスチャ呼び出し
	Texture* CallTexture(const string& textureName);

	//テクスチャ削除
	void deleteTextures(const string& textureName);

private:
	//初期化
	void Initialize(DirectXCommon* dxCommon);

	//メンバ変数
private:
	//テクスチャ配列
	std::vector<Texture*> textures;

protected:
	//コンストラクタ
	TextureManager();

};

