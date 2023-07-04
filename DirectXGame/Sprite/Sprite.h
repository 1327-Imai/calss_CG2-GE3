#pragma once
#include "SpriteCommon.h"
#include "WorldTransform.h"
#include "Texture.h"

class Sprite {
public:

	//コンストラクタ
	Sprite();

	//デストラクタ
	~Sprite();

	//メンバ関数
	void Initialize(SpriteCommon* spriteCommon, Texture* texture);

	void Update();

	void Draw();

	void SetTexture(Texture* texture);

	//アクセッサ
	void SetPosition(const Vector2 position) {
		position_ = position;
	}
	Vector2 GetPosition() const {
		return position_;
	}

	void SetRotation(float rotation) {
		rotation_ = rotation;
	}
	float GetRotation() const {
		return rotation_;
	}

	void SetScale(const Vector2 scale) {
		scale_ = scale;
	}
	Vector2 GetScale() const {
		return scale_;
	}

	void SetSize(const Vector2 size) {
		size_ = size;
	}
	Vector2 GetSize() const {
		return size_;
	}

	void SetAnchorPoint(const Vector2 anchorPoint) {
		anchorPoint_ = anchorPoint;
	}
	Vector2 GetAnchorPoint() const {
		return anchorPoint_;
	}

	void SetColor(Vector4 color) {
		color_ = color;
	}
	Vector4 GetColor() const {
		return color_;
	}

	void SetIsFlipX(bool isFlipX) {
		isFlipX_ = isFlipX;
	}
	bool GetIsFlipX() const {
		return isFlipX_;
	}

	void SetIsFlipY(bool isFlipY) {
		isFlipY_ = isFlipY;
	}
	bool GetIsFlipY() const {
		return isFlipY_;
	}

	void SetIsVisible(bool isVisible) {
		isVisible_ = isVisible;
	}
	bool GetIsVisible() const {
		return isVisible_;
	}

	void SetTextureSize(Vector2 textureSize) {
		textureSize_ = textureSize;
	}
	Vector2 GetTextureSize() {
		return textureSize_;
	}

	void AdujustTextureSize();

private:
	void CreateConstMapTransform();

	void CreateConstMapMaterial();


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
	DirectXCommon* dxCommon_ = nullptr;

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

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRVヒープの先頭ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

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

	//テクスチャ
	Texture* texture_ = nullptr;

};

