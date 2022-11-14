#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXmath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <wrl.h>


#include <math.h>
#include <random>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"
#include "Input.h"
#include "DX12base.h"

using namespace DirectX;
using namespace Microsoft::WRL;

const double PI = 3.141592;

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
std::uniform_real_distribution<float> distPosX(-100.0 , 100.0);
std::uniform_real_distribution<float> distPosY(-50.0 , 50.0);
std::uniform_real_distribution<float> distPosZ(30.0 , 60.0);
std::uniform_real_distribution<float> distRot(0 , XMConvertToRadians(360.0f));



#pragma region//構造体

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; //色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D変換行列
};

struct Object3D {

	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファマッピング(行列用)
	ConstBufferDataTransform* constMapTransform = nullptr;
	//アフィン変換情報
	XMFLOAT3 scale = {1 , 1 , 1};
	XMFLOAT3 rotation = {0 , 0 , 0};
	XMFLOAT3 position = {0 , 0 , 0};
	//ワールド変換行列
	XMMATRIX matWorld = XMMatrixIdentity();
	//親オブジェクトへのポインタ
	Object3D* parent = nullptr;

};

#pragma endregion


#pragma region//関数のプロトタイプ宣言
////ウィンドウプロシーシャ
//LRESULT WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam);

//3Dオブジェクト初期化
void InitializeObject3d(Object3D* object , ComPtr<ID3D12Device> device);

void UpdataObject3d(Object3D* object , XMMATRIX& matView , XMMATRIX& matProjection);

void DrawObject3d(Object3D* object , ComPtr<ID3D12GraphicsCommandList> commandlist , D3D12_VERTEX_BUFFER_VIEW& vbView , D3D12_INDEX_BUFFER_VIEW& ibView , UINT numIndices);

void MoveObject3d(Object3D* object , Input* input);

void SetRandomPositionObject3d(Object3D* object);
void SetRandomRotateObject3d(Object3D* object);

#pragma endregion//関数のプロトタイプ宣言

//main関数
int WINAPI WinMain(_In_ HINSTANCE , _In_opt_ HINSTANCE , _In_ LPSTR , _In_ int) {
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!/n");


#pragma region//ウィンドウの生成

	//WindowsAPI
	WinApp* winApp = new WinApp;
	winApp->Initialize();

#pragma endregion//ウィンドウの生成

#pragma region//メッセージループ
	MSG msg{}; //メッセージ

#pragma region//DirectX初期化処理
	//デバッグレイヤーの有効化

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif

	//DirectX基礎
	DX12base* dx12Base_ = new DX12base;
	dx12Base_->Initialize(winApp);

	//DirectX初期化処理
	HRESULT result;

#pragma region// キーボードデバイスの生成

	Input* input = new Input();
	input->Initialize(winApp);

#pragma endregion

#pragma endregion//DirectX初期化処理

#pragma region//描画初期化処理

#pragma region//頂点データ

	struct Vertex {
		XMFLOAT3 pos;	//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;	//uv座標
	};

	//頂点データ
	Vertex vertices[] = {
		//前
		{{-5.0f , -5.0f , -5.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{-5.0f , 5.0f , -5.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{5.0f , -5.0f , -5.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{5.0f , 5.0f , -5.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//後
		{{-5.0f , -5.0f , 5.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{-5.0f , 5.0f , 5.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{5.0f , -5.0f , 5.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{5.0f , 5.0f , 5.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//左
		{{-5.0f , -5.0f , -5.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{-5.0f , -5.0f , 5.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{-5.0f , 5.0f , -5.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{-5.0f , 5.0f , 5.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//右
		{{5.0f , -5.0f , -5.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{5.0f , -5.0f , 5.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{5.0f , 5.0f , -5.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{5.0f , 5.0f , 5.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//上
		{{-5.0f , 5.0f , -5.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{5.0f , 5.0f , -5.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{-5.0f , 5.0f , 5.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{5.0f , 5.0f , 5.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//下
		{{-5.0f , -5.0f , -5.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{5.0f , -5.0f , -5.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{-5.0f , -5.0f , 5.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{5.0f , -5.0f , 5.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3

	};

	//インデックスデータ
	uint16_t indices[] = {
		//前
		0 , 1 , 2 , //三角形1つ目
		1 , 3 , 2 , //三角形2つ目
		//後ろ
		4 , 6 , 5 ,
		5 , 6 , 7 ,
		//左
		8 , 9 , 10 ,
		9 , 11 , 10 ,
		//右
		12 , 14 , 13 ,
		13 , 14 , 15 ,
		//上
		16 , 18 , 17 ,
		17 , 18 , 19 ,
		//下
		20 , 21 , 22 ,
		21 , 23 , 22 ,
	};

	//法線の計算
	for (int i = 0; i < _countof(indices) / 3; i++) {

		//三角形のインデックスを取り出して一時的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1 , p0);
		XMVECTOR v2 = XMVectorSubtract(p2 , p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1 , v2);

		//正規化
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal , normal);
		XMStoreFloat3(&vertices[index1].normal , normal);
		XMStoreFloat3(&vertices[index2].normal , normal);

	}


	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	result = dx12Base_->GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	result = dx12Base_->GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&indexBuff)
	);
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0 , nullptr , (void**)&vertMap);
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];	//座標をコピー
	}
	//繋がりを解除
	vertBuff->Unmap(0 , nullptr);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0 , nullptr , (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];	//座標をコピー
	}
	//繋がりを解除
	indexBuff->Unmap(0 , nullptr);

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点１つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region//頂点シェーダー
	//頂点シェーダーファイルの読み込みとコンパイル
	ID3DBlob* vsBlob = nullptr;		//頂点シェーダーオブジェクト
	ID3DBlob* psBlob = nullptr;		//ピクセルシェーダーオブジェクト
	ID3DBlob* errorBlob = nullptr;	//エラーオブジェクト

	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl" ,									//シェーダーファイル名
		nullptr ,
		D3D_COMPILE_STANDARD_FILE_INCLUDE ,					//インクルード可能にする
		"main" ,											//エントリーポイント名
		"vs_5_0" ,											//シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION ,	//デバッグ用設定
		0 ,
		&vsBlob ,
		&errorBlob
	);

	//シェーダーコードのエラー
	//エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer() ,
					errorBlob->GetBufferSize() ,
					error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region//ピクセルシェーダー
	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl" ,									//シェーダーファイル名
		nullptr ,
		D3D_COMPILE_STANDARD_FILE_INCLUDE ,					//インクルード可能にする
		"main" ,												//エントリーポイント名
		"ps_5_0" ,											//シェーダーモデル設定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION ,	//デバッグ用設定
		0 ,
		&psBlob ,
		&errorBlob
	);

	//シェーダーコードのエラー
	//エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer() ,
					errorBlob->GetBufferSize() ,
					error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz座標
			"POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
		{//法線ベクトル
			"NORMAL" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
		{//uv座標
			"TEXCODE" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
	};

#pragma region//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定

	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;			//背面化リング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;			//ポリゴン内塗りつぶし
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;	//ワイヤーフレーム表示
	pipelineDesc.RasterizerState.DepthClipEnable = true;					//深度クリッピングを有効に

	//ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//共通設定(アルファ値)
	blenddesc.BlendEnable = false;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;								//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;		//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;									//深度値フォーマット

	//その他の設定
	pipelineDesc.NumRenderTargets = 1;								//描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1;								//1ピクセルにつき1回のサンプリング
#pragma endregion

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma region//ルートパラメータ
	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
#pragma endregion

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

#pragma region//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;					//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc ,
		D3D_ROOT_SIGNATURE_VERSION_1_0 ,
		&rootSigBlob ,
		&errorBlob);
	assert(SUCCEEDED(result));

	result = dx12Base_->GetDevice()->CreateRootSignature(
		0 ,
		rootSigBlob->GetBufferPointer() ,
		rootSigBlob->GetBufferSize() ,
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();
#pragma endregion

	//パイプラインステートの生成
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	result = dx12Base_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc , IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma region//定数バッファ

	//定数バッファの生成
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//定数バッファの生成
	result = dx12Base_->GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0 , nullptr , (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	//定数バッファへのデータ転送
	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1.0f , 1.0f , 1.0f , 0.5f);

	//定数バッファの生成
	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

#pragma endregion

#pragma region//射影変換
	//単位行列を代入
	//constMapTransform->mat = XMMatrixIdentity();

	////平行投影変換行列の計算
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
	//	0.0f , WINDOW_WIDTH ,
	//	WINDOW_HEIGHT , 0.0f ,
	//	0.0f , 1.0f
	//);

	//透視投影変換行列の計算
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)WinApp::WINDOW_WIDTH / WinApp::WINDOW_HEIGHT ,
		0.1f , 1000.0f
	);
#pragma endregion

#pragma region//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0.0f , 0.0f , -100.0f);	//視点座標
	XMFLOAT3 target(0.0f , 0.0f , 0.0f);	//注視点座標
	XMFLOAT3 up(0.0f , 1.0f , 0.0f);		//上方向ベクトル
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye) , XMLoadFloat3(&target) , XMLoadFloat3(&up));

	float angle = 0.0f;

#pragma endregion

#pragma region//ワールド変換行列

	//3Dオブジェクトの数
	const size_t kObjectCount = 50;

	Object3D object3ds[kObjectCount];

	for (int i = 0; i < _countof(object3ds); i++) {

		//初期化
		InitializeObject3d(&object3ds[i] , dx12Base_->GetDevice());

		//ここから下は親子構造のサンプル
		//先頭以外なら
		if (i > 0) {
			//ひとつ前のオブジェクトを親オブジェクトとする
			//object3ds[i].parent = &object3ds[i - 1];
			//親オブジェクトの9割の大きさ
			//object3ds[i].scale = {0.9f , 0.9f , 0.9f};
			//親オブジェクトに対してZ軸まわりに30度回転
			//object3ds[i].rotation = {0.0f , 0.0f , XMConvertToRadians(30.0f)};

			//親オブジェクトに対してZ軸方向に-8.0ずらす
			//object3ds[i].position = {0.0f , 0.0f , -8.0f};

			//ランダムに配置する
			SetRandomPositionObject3d(&object3ds[i]);
			SetRandomRotateObject3d(&object3ds[i]);
		}

	}

#pragma endregion

#pragma region//テクスチャ
	//画像イメージデータの作成
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/texture.jpg" ,
		WIC_FLAGS_NONE ,
		&metadata ,
		scratchImg
	);

	//画像イメージデータの作成
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/reimu.png" ,
		WIC_FLAGS_NONE ,
		&metadata2 ,
		scratchImg2
	);


	ScratchImage mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages() ,
		scratchImg.GetImageCount() ,
		scratchImg.GetMetadata() ,
		TEX_FILTER_DEFAULT ,
		0 ,
		mipChain
	);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	ScratchImage mipChain2{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg2.GetImages() ,
		scratchImg2.GetImageCount() ,
		scratchImg2.GetMetadata() ,
		TEX_FILTER_DEFAULT ,
		0 ,
		mipChain2
	);
	if (SUCCEEDED(result)) {
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata2.format = MakeSRGB(metadata2.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc{};
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;	//幅
	textureResouceDesc.Height = (UINT)metadata.height;	//高さ
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc2{};
	textureResouceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc2.Format = metadata2.format;
	textureResouceDesc2.Width = metadata2.width;	//幅
	textureResouceDesc2.Height = (UINT)metadata2.height;	//高さ
	textureResouceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResouceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResouceDesc2.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff = nullptr;
	result = dx12Base_->GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	//テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff2 = nullptr;
	result = dx12Base_->GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc2 ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff2)
	);
	assert(SUCCEEDED(result));

	//テクスチャバッファにデータ転送
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i , 0 , 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//全領域へコピー
			img->pixels ,			//元データアドレス
			(UINT)img->rowPitch ,	//1ラインサイズ
			(UINT)img->slicePitch	//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	//全ミップマップについて
	for (size_t i = 0; i < metadata2.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img2 = scratchImg2.GetImage(i , 0 , 0);
		//テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//全領域へコピー
			img2->pixels ,			//元データアドレス
			(UINT)img2->rowPitch ,	//1ラインサイズ
			(UINT)img2->slicePitch	//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

#pragma endregion

#pragma region//シェーダーリソースビュー
	//デスクリプタヒープの生成
	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	result = dx12Base_->GetDevice()->CreateDescriptorHeap(
		&srvHeapDesc ,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));

	//デスクリプタハンドル
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダーリソースビューの作成
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dx12Base_->GetDevice()->CreateShaderResourceView(texBuff.Get() , &srvDesc , srvHandle);

	//二枚目
	//SRVヒープの先頭ハンドルを取得
	UINT incremantSize = dx12Base_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incremantSize;

	//シェーダーリソースビューの作成
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};	//設定構造体
	srvDesc2.Format = textureResouceDesc2.Format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc2.Texture2D.MipLevels = textureResouceDesc2.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dx12Base_->GetDevice()->CreateShaderResourceView(texBuff2.Get() , &srvDesc2 , srvHandle);

#pragma endregion

#pragma endregion//描画初期化処理

#pragma region//ゲームループ
	while (true) {

#pragma region//ウィンドウメッセージ処理

		if (winApp->ProcMessage()) {
			break;
		}

#pragma endregion//ウィンドウメッセージ処理

#pragma region//DirectX毎フレーム処理

		input->Update();


#pragma region//更新処理

		//ビュー変換
		//いずれかのキーを押していたら
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {

			//押したキーに応じてangleを増減させる
			if (input->PushKey(DIK_D)) {
				angle += XMConvertToRadians(1.0f);
			}
			if (input->PushKey(DIK_A)) {
				angle -= XMConvertToRadians(1.0f);
			}

			//angleラジアンだけY軸周りに回転。半径は-100
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			//ビュー変換行列を作り直す
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye) , XMLoadFloat3(&target) , XMLoadFloat3(&up));

		}

		MoveObject3d(&object3ds[0] , input);

		for (int i = 0; i < _countof(object3ds); i++) {
			UpdataObject3d(&object3ds[i] , matView , matProjection);
		}

#pragma endregion//更新処理

#pragma region//描画処理
		dx12Base_->PreDraw();

		//パイプラインステートとルートシグネチャの設定コマンド
		dx12Base_->GetCmdList()->SetPipelineState(pipelineState.Get());
		dx12Base_->GetCmdList()->SetGraphicsRootSignature(rootSignature.Get());

		//プリミティブ形状の設定コマンド
		dx12Base_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//頂点バッファビューの設定コマンド
		dx12Base_->GetCmdList()->IASetVertexBuffers(0 , 1 , &vbView);

		//インデックスバッファビューの設定コマンド
		dx12Base_->GetCmdList()->IASetIndexBuffer(&ibView);

		//頂点バッファ―ビューをセットするコマンド
		dx12Base_->GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial->GetGPUVirtualAddress());

		//SRVヒープの設定コマンド
		dx12Base_->GetCmdList()->SetDescriptorHeaps(1 , srvHeap.GetAddressOf());

		//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		if (input->PushKey(DIK_SPACE)) {
			//二枚目を指し示すように差し込む
			srvGpuHandle.ptr += incremantSize;
		}

		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		dx12Base_->GetCmdList()->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);

		for (int i = 0; i < _countof(object3ds); i++) {
			DrawObject3d(&object3ds[i] , dx12Base_->GetCmdList() , vbView , ibView , _countof(indices));
		}

		dx12Base_->PostDraw();
#pragma endregion//描画処理

#pragma endregion//DirectX毎フレーム処理

	}
#pragma endregion//ゲームループ

	//ID3D12DebugDevice* debugInterface;

	//if (SUCCEEDED(device.Get()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}

	delete input;

	winApp->Finalize();
	delete winApp;

#pragma endregion//メッセージループ

	return 0;
}


#pragma region//関数の定義
//ウィンドウプロシーシャ
LRESULT WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam) {

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
	//OSに対して、アプリの終了を伝える
	PostQuitMessage(0);
	return 1;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd , msg , wparam , lparam);
}


//3Dオブジェクト初期化
void InitializeObject3d(Object3D* object , ComPtr<ID3D12Device> device) {

	HRESULT result;

	//定数バッファの生成
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//定数バッファの生成
	result = device.Get()->CreateCommittedResource(
		&cbHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&object->constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = object->constBuffTransform->Map(0 , nullptr , (void**)&object->constMapTransform); // マッピング
	assert(SUCCEEDED(result));
}

void UpdataObject3d(Object3D* object , XMMATRIX& matView , XMMATRIX& matProjection) {

	XMMATRIX matScale , matRot , matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(object->scale.x , object->scale.y , object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationX(object->rotation.x);
	matRot *= XMMatrixRotationY(object->rotation.y);
	matTrans = XMMatrixTranslation(object->position.x , object->position.y , object->position.z);

	//ワールド行列の合成
	object->matWorld = XMMatrixIdentity();
	object->matWorld *= matScale;
	object->matWorld *= matRot;
	object->matWorld *= matTrans;

	//親オブジェクトがあれば
	if (object->parent != nullptr) {
		//親オブジェクトのワールド行列を掛ける
		object->matWorld *= object->parent->matWorld;
	}

	//定数バッファへデータ転送
	object->constMapTransform->mat = object->matWorld * matView * matProjection;

}

void DrawObject3d(Object3D* object , ComPtr<ID3D12GraphicsCommandList> commandlist , D3D12_VERTEX_BUFFER_VIEW& vbView , D3D12_INDEX_BUFFER_VIEW& ibView , UINT numIndices) {
	//頂点バッファの設定
	commandlist->IASetVertexBuffers(0 , 1 , &vbView);
	//インデックスバッファの設定
	commandlist->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	commandlist->SetGraphicsRootConstantBufferView(2 , object->constBuffTransform->GetGPUVirtualAddress());
	//描画コマンド
	commandlist->DrawIndexedInstanced(numIndices , 1 , 0 , 0 , 0);
}

void MoveObject3d(Object3D* object , Input* input) {
	if (input->PushKey(DIK_UP) ||
		input->PushKey(DIK_DOWN) ||
		input->PushKey(DIK_RIGHT) ||
		input->PushKey(DIK_LEFT)) {

		if (input->PushKey(DIK_UP)) {
			object->position.y += 1.0f;
		}
		if (input->PushKey(DIK_DOWN)) {
			object->position.y -= 1.0f;
		}
		if (input->PushKey(DIK_RIGHT)) {
			object->position.x += 1.0f;
		}
		if (input->PushKey(DIK_LEFT)) {
			object->position.x -= 1.0f;
		}
	}
}

void SetRandomPositionObject3d(Object3D* object) {
	object->position.x = distPosX(engine);
	object->position.y = distPosY(engine);
	object->position.z = distPosZ(engine);
}

void SetRandomRotateObject3d(Object3D* object) {
	object->rotation.x = distRot(engine);
	object->rotation.y = distRot(engine);
	object->rotation.z = distRot(engine);
}

#pragma endregion//関数の定義