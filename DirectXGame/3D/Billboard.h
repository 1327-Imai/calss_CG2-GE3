#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "Camera.h"

class Billboard
{
public:
	//コンストラクタ
	Billboard();

	//デストラクタ
	~Billboard();

	//構造体
private:
	struct VertexPosNormalUv {
		Vector3 pos;	//xyz座標
		Vector3 normal;	//法線ベクトル
		Vector2 uv;	//uv座標
	};

	//静的メンバ関数
public:
	static void StaticInitialize(DirectXCommon* dxCommon , Camera* camera);

	static void SetCamera(Camera* camera) {
		Billboard::camera_ = camera;
	}

private:
	static void CreateGraphicsPipeline();

	static void CreateModel();

	//アクセッサ
	static void SetDevice(ID3D12Device* device) {
		Billboard::device_ = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Billboard::cmdList_ = cmdList;
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

	void SetPosition(Vector3 position) {
		worldTransform_.translation_ = position;
	}
	Vector3 GetPosition() const {
		Vector3 pos = {
			worldTransform_.matWorld_.m[3][0] ,
			worldTransform_.matWorld_.m[3][1] ,
			worldTransform_.matWorld_.m[3][2]
		};
		return pos;
	}

	void SetRotation(Vector3 rotation) {
		worldTransform_.rotation_ = rotation;
	}
	Vector3 GetRotation() {
		return worldTransform_.rotation_;
	}

	void SetScale(Vector3 scale) {
		worldTransform_.scale_ = scale;
	}
	Vector3 GetScale() {
		return worldTransform_.scale_;
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
	static ID3D12Device* device_;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature_;
	//パイプラインステート
	static ComPtr<ID3D12PipelineState> pipelineState_;
	//頂点数
	static const int vertexCount_ = 4;
	//頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//頂点配列
	static VertexPosNormalUv vertices[vertexCount_];
	//インデックス数
	static const int indexCount_ = 3 * 2;
	//インデックス配列
	static uint16_t indices[indexCount_];
	//インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	//インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	//リソース設定
	static D3D12_RESOURCE_DESC resDesc;

	//カメラ
	static Camera* camera_;

private:
	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBufferTransform_ = nullptr;
	//定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBufferMaterial_ = nullptr;

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
	//SRVヒープの先頭ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

	//ワールド変換
	WorldTransform worldTransform_;
	//テクスチャ
	Texture* texture_;

};

