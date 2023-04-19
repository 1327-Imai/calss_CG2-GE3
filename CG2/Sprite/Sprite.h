#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"

class Sprite {
public:

	//コンストラクタ
	Sprite();

	//デストラクタ
	~Sprite();

	//メンバ関数
	void Initialize(SpriteCommon* spriteCommon,uint32_t textureIndex = UINT32_MAX);

	void Update();

	void Draw();

	void SetPosition(const Vector2 position);
	Vector2 GetPosition() const;

	void SetRotation(float rotation);
	float GetRotation() const;

	void SetSize(const Vector2 size);
	Vector2 GetSize() const;

	void SetScale(const Vector2 scale);
	Vector2 GetScale() const;

	void SetAnchorPoint(const Vector2 anchorPoint);
	Vector2 GetAnchorPoint() const;

	void SetColor(Vector4 color);
	Vector4 GetColor() const;

	void SetIsFlipX(bool isFlipX);
	bool GetIsFlipX() const;

	void SetIsFlipY(bool isFlipY);
	bool GetIsFlipY() const;

	void SetIsVisible(bool isVisible);
	bool GetIsVisible() const;

	void SetTextureIndex(uint32_t textureIndex);
	uint32_t GetTextureIndex();

	void setTextureSize(Vector2 textureSize);
	Vector2 GetTextureSize();

private:
	void CreateConstMapTransform();

	void CreateConstMapMaterial();

	void AdujustTextureSize();

	//構造体
private:
	struct ConstBufferDataMaterial {
		Vector4 color;
	};

	struct ConstBufferDataTransform {
		Matrix4 mat;
	};

	struct Vertex {
		Vector3 pos;	//xyz座標
		Vector2 uv;		//uv座標
	};

	//列挙型
private:
	enum VertexNumber {
		LB,	//左下:0
		LT,	//左上:1
		RB,	//右下:2
		RT,	//右上:3
	};

	//メンバ変数
private:
	//DirectX基礎
	DX12base* dx12base_ = nullptr;

	//スプライト共通処理
	SpriteCommon* spriteCommon_ = nullptr;;

	Vertex* vertMap_ = nullptr;

	//マテリアル定数バッファ
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	//マテリアル定数バッファビュー
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;

	//
	ConstBufferDataTransform* constMapTransform_ = nullptr;

	//
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//テクスチャインデックス
	uint32_t textureIndex_;

	//ワールド変換
	WorldTransform worldTransform_;

	//2D座標
	Vector2 position_;

	//回転
	float rotation_;

	//表示サイズ
	Vector2 size_;

	//スケール
	Vector2 scale_;

	//アンカー
	Vector2 anchorPoint_;

	//色
	Vector4 color_;

	//X軸フリップ
	bool isFlipX_ = false;

	//Y軸フリップ
	bool isFlipY_ = false;

	//表示フラグ
	bool isVisible_ = true;

	//テクスチャ左上座標
	Vector2 textureleftTop_ = {0 , 0};

	//テクスチャ切り出しサイズ
	Vector2 textureSize_;

	//頂点データ
	Vertex vertices_[4];
};

