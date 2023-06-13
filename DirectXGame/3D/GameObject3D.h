#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Texture.h"
#include "DX12base.h"

#include "MathFunc.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "LightGroup.h"


class GameObject3D {

public:
	//メンバ変数
	void PreLoadModel(const char* modelFileName);
	void PreLoadTexture(const wchar_t* textureFileName);

	void Initialize();

	void Update();

	void Draw();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//アクセッサ
	void SetViewProjection(ViewProjection* viewProjection){this->viewProjection=viewProjection;}
	void SetMatProjection(Matrix4* matProjection);

	/// <summary>
	/// ライトのセット
	/// </summary>
	static void SetLight(LightGroup* light) { GameObject3D::light = light; }

private:
	void InitializeConstMapTransform();
	void InitializeConstMapMaterial();

	//構造体
private:
	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		Vector3 ambient;  // アンビエント係数
		float pad1;        // パディング
		Vector3 diffuse;  // ディフューズ係数
		float pad2;        // パディング
		Vector3 specular; // スペキュラー係数
		float pad3;        // パディング

		float alpha;       // アルファ
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform {
		//Matrix4 mat; //3D変換行列
		Matrix4 viewprojection;// ビュープロジェクション行列
		Matrix4 world;// ワールド行列
		Vector3 cameraPos;// カメラ座標（ワールド座標）
	};

	//メンバ変数
public:
	//ワールド変換
	WorldTransform worldTransform;


private:
	//モデル
	Model model;
	//モデルのファイル名
	const char* modelFileName = nullptr;

	//テクスチャ
	Texture textrue;
	//モデルのファイル名
	const wchar_t* textureFileName = nullptr;

	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファマッピング(行列用)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbTransformHeapProp;
	D3D12_HEAP_PROPERTIES cbMaterialHeapProp{};

	//リソース設定
	D3D12_RESOURCE_DESC cbTransformResourceDesc;
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	Matrix4* matProjection;

	//DirectX基礎部分
	DX12base& dx12base = DX12base::GetInstance();
	//ビュープロジェクション
	ViewProjection* viewProjection;

	Vector3 ambient;            // アンビエント影響度
	Vector3 diffuse;            // ディフューズ影響度
	Vector3 specular;           // スペキュラー影響度
	float alpha;                 // アルファ

	static LightGroup* light;
};

