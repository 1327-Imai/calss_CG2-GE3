#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "Camera.h"

#include <forward_list>

class ParticleManager
{
public:
	//コンストラクタ
	ParticleManager();

	//デストラクタ
	~ParticleManager();

	//構造体
private:
	struct VertexPos {
		Vector3 pos;	//xyz座標
		float scale;
	};

	struct Particle {

		Vector3 pos;		//座標
		Vector3 velocity;	//速度
		Vector3 accel;		//加速度
		int flame = 0;		//現在フレーム
		int eFlame = 0;		//終了フレーム
		float scale = 1.0f;	//現在スケール
		float sScale = 1.0f;//初期スケール
		float eScale = 0.0f;//最終スケール

	};

	//静的メンバ関数
public:
	static void StaticInitialize(DirectXCommon* dxCommon , Camera* camera);

	static void SetCamera(Camera* camera) {
		ParticleManager::camera_ = camera;
	}

private:
	static void CreateGraphicsPipeline();

	static void CreateModel();

	//アクセッサ
	static void SetDevice(ID3D12Device* device) {
		ParticleManager::device_ = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		ParticleManager::cmdList_ = cmdList;
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

	void Add(int life , Vector3 position , Vector3 velocity , Vector3 accel,float startSclae, float endScale);

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
		Matrix4 matBillboard;
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
	static const int vertexCount_ = 1024;
	//頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
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

	std::forward_list<Particle> particles;

	//テクスチャ
	Texture* texture_;

};

