#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXmath.h>
#include <wrl.h>
#include <chrono>
#include <thread>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#include <cassert>
#include <vector>
#include <string>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class DX12base{

public:
	//メンバ関数
	//初期化処理
	void Initialize(WinApp* winApp);

	void PreDraw();

	void PostDraw();

private:

	void CreateCmdList();

	void SetSwapChain();

	void DepthTest();

	void CreateFence();

	void InitializeFixFPS();

	void UpdateFixFPS();

public:
	//アクセッサ
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();

	ComPtr<ID3D12Device> GetDevice();

	//メンバ変数
private:
	HRESULT result;

	//DirectX初期化処理
	ComPtr<ID3D12Debug> debugController_;
	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;

	//アダプターの列挙例
	std::vector<ComPtr<IDXGIAdapter4>> adapters_;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter_ = nullptr;

	D3D_FEATURE_LEVEL featureLevel_;

	//コマンドキュー
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

#pragma region//スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	
	//IDXGISwapChain1のComPtrを用意
	ComPtr<IDXGISwapChain1> swapChain1_;

	//デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};

	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

#pragma endregion

#pragma region//深度テスト
	//デスクリプタ
	D3D12_RESOURCE_DESC depthResourceDesc_{};

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp_{};

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue_{};

	//リソース
	ComPtr<ID3D12Resource> depthBuff_ = nullptr;

	//深度ビュー用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc_{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_ = {};

#pragma endregion

#pragma region//フェンス

	//フェンスの生成
	ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64 fenceVal_ = 0;

#pragma endregion

#pragma region//描画処理

	//バックバッファ
	UINT bbIndex_;

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	//レンダーターゲットビューのハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

	//クリアカラー
	FLOAT clearColor_[4] = {0.1f , 0.25f , 0.5f , 0.0f};;

	//ビューポート
	D3D12_VIEWPORT viewport_{};

	//シザー矩形
	D3D12_RECT scissorRect_{};

#pragma endregion

	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	//WinAppのポインタ
	WinApp* winApp_ = nullptr;
};

