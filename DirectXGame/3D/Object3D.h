#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Texture.h"

class Object3D
{
public:
	//コンストラクタ
	Object3D();

	//デストラクタ
	~Object3D();

public://静的メンバ関数
	static void StaticInitialize(DirectXCommon* dxCommon , ViewProjection* viewProjection);

	static void CreateGraphicsPipeline();

	//アクセッサ
	static void SetDevice(ID3D12Device* device) {
		Object3D::device = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Object3D::cmdList = cmdList;
	}
	static void SetViewProjection(ViewProjection* viewProjection) {
		Object3D::viewProjection = viewProjection;
	}

	//メンバ関数
public:
	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	//描画処理
	void Draw();

	void SetTexture(Texture* texture);

	//アクセッサ
	void SetModel(Model* model) {
		this->model = model;
	}

	void SetPosition(Vector3 position) {
		worldTransform.translation_ = position;
	}
	Vector3 GetPosition() const {
		Vector3 pos = {
			worldTransform.matWorld_.m[3][0] ,
			worldTransform.matWorld_.m[3][1] ,
			worldTransform.matWorld_.m[3][2]
		};
		return pos;
	}

	void SetRotation(Vector3 rotation) {
		worldTransform.rotation_ = rotation;
	}
	Vector3 GetRotation() {
		return worldTransform.rotation_;
	}

	void SetScale(Vector3 scale) {
		worldTransform.scale_ = scale;
	}
	Vector3 GetScale() {
		return worldTransform.scale_;
	}


private:
	//定数バッファの生成
	void CreateConstBufferTransform();
	void CreateConstBufferMaterial();

	//定数バッファの更新
	void UpdateConstBufferTransform();

private:
	//構造体
	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		Vector4 color; //色(RGBA)
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform {
		Matrix4 mat; //3D変換行列
	};

private:
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	//パイプラインステート
	static ComPtr<ID3D12PipelineState> pipelineState;
	//ビュープロジェクション
	static ViewProjection* viewProjection;

private:
	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBufferTransform = nullptr;
	//定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBufferMaterial = nullptr;

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRVヒープの先頭ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	//ワールド変換
	WorldTransform worldTransform;
	//モデル
	Model* model;
	//テクスチャ
	Texture* texture;

};

