#include "DX12base.h"

void DX12base::Initialize(WinApp* winApp) {
	
	//nullptrチェック
	assert(winApp);

	winApp_ = winApp;

	//デバッグレイヤーの有効化

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		debugController_->EnableDebugLayer();
	}
#endif

	//DirectX初期化処理

	//アダプタの列挙
	//DX6Iファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	//パフォーマンスが高いものをから順に、すべてのアダプターを列挙する
	for (UINT i = 0;
		 dxgiFactory_->EnumAdapterByGpuPreference(i ,
		 DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE ,
		 IID_PPV_ARGS(&tmpAdapter_)) != DXGI_ERROR_NOT_FOUND;
		 i++) {
		//動的配列につかする
		adapters_.push_back(tmpAdapter_);
	}

	//妥当なアダプタを選別する
	for (size_t i = 0; i < adapters_.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//アダプターの情報を取得する
		adapters_[i]->GetDesc3(&adapterDesc);

		//ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//デバイスを採用してループを抜ける
			tmpAdapter_ = adapters_[i];
			break;
		}
	}

	//デバイスの生成
	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1 ,
		D3D_FEATURE_LEVEL_12_0 ,
		D3D_FEATURE_LEVEL_11_1 ,
		D3D_FEATURE_LEVEL_11_0 ,
	};

	for (size_t i = 0; i < _countof(levels); i++) {
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(
			tmpAdapter_.Get() ,
			levels[i] ,
			IID_PPV_ARGS(&device_)
		);
		if (result == S_OK) {
			//デバイスを生成できた時点でループを抜ける
			featureLevel_ = levels[i];
			break;
		}
	}

	CreateCmdList();
	SetSwapChain();
	DepthTest();
	CreateFence();
}

void DX12base::PreDraw() {
	//バックバッファの番号を取得
	bbIndex_ = swapChain_->GetCurrentBackBufferIndex();

	//1.リソースバリアで書き込み可能に変更
	barrierDesc_.Transition.pResource = backBuffers_[bbIndex_].Get();			//バックバッファを指定
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		//表示状態から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態へ
	commandList_->ResourceBarrier(1 , &barrierDesc_);

	//2.描画先の変更
	//レンダーターゲットビューのハンドルを取得
	rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle_.ptr += static_cast<unsigned long long>(bbIndex_) * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
	dsvHandle_ = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1 , &rtvHandle_ , false , &dsvHandle_);

	//3.画面クリア
	//レンダーターゲットクリア
	commandList_->ClearRenderTargetView(rtvHandle_ , clearColor_ , 0 , nullptr);
	
	//深度バッファクリア
	commandList_->ClearDepthStencilView(dsvHandle_ , D3D12_CLEAR_FLAG_DEPTH , 1.0f , 0 , 0 , nullptr);

	//4.描画コマンド
	//ビューポート設定のコマンド
	viewport_.Width = winApp_->WINDOW_WIDTH;			//横幅
	viewport_.Height = winApp_->WINDOW_HEIGHT - 0;	//縦幅

	viewport_.TopLeftX = 0;					//左上X
	viewport_.TopLeftY = 0;					//左上Y
	viewport_.MinDepth = 0.0f;				//最小深度
	viewport_.MaxDepth = 1.0f;				//最大深度

	//ビューポート設定コマンドを、コマンドリストに積む
	commandList_->RSSetViewports(1 , &viewport_);

	//シザー矩形
	scissorRect_.left = 0;											//切り抜き座標左
	scissorRect_.right = scissorRect_.left + winApp_->WINDOW_WIDTH;	//切り抜き座標右
	scissorRect_.top = 0;											//切り抜き座標上
	scissorRect_.bottom = scissorRect_.top + winApp_->WINDOW_HEIGHT;	//切り抜き座標下

	//シザー矩形コマンドを、コマンドリストに積む
	commandList_->RSSetScissorRects(1 , &scissorRect_);

}

void DX12base::PostDraw() {
	//バックバッファの番号を取得
	bbIndex_ = swapChain_->GetCurrentBackBufferIndex();

	//5.リソースバリアを戻す
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//表示状態へ
	commandList_->ResourceBarrier(1 , &barrierDesc_);

	//命令のクローズ
	result = commandList_->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* commandLists_[] = {commandList_.Get()};
	commandQueue_->ExecuteCommandLists(1 , commandLists_);

	//画面に表示するバッファをフリップ(裏表の入れ替え)
	result = swapChain_->Present(1 , 0);
	assert(SUCCEEDED(result));

	//コマンドの実行完了を待つ
	commandQueue_->Signal(fence_.Get() , ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr , false , false , nullptr);
		fence_->SetEventOnCompletion(fenceVal_ , event);
		if (event != 0) {
			WaitForSingleObject(event , INFINITE);
			CloseHandle(event);
		}
	}

	//キューをクリア
	result = cmdAllocator_->Reset();
	assert(SUCCEEDED(result));
	//再びコマンドリストをためる準備
	result = commandList_->Reset(cmdAllocator_.Get() , nullptr);

}

void DX12base::CreateCmdList() {

	//コマンドアロケータを生成
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		IID_PPV_ARGS(&cmdAllocator_)
	);
	assert(SUCCEEDED(result));

	//コマンドリストを生成
	result = device_->CreateCommandList(
		0 ,
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		cmdAllocator_.Get() , nullptr ,
		IID_PPV_ARGS(&commandList_)
	);
	assert(SUCCEEDED(result));

	//コマンドキュー
	//コマンドキューを生成
	result = device_->CreateCommandQueue(
		&commandQueueDesc_ ,
		IID_PPV_ARGS(&commandQueue_)
	);
	assert(SUCCEEDED(result));

}

void DX12base::SetSwapChain() {

	swapChainDesc_.Width = winApp_->WINDOW_WIDTH;
	swapChainDesc_.Height = winApp_->WINDOW_HEIGHT;
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				//色情報の書式
	swapChainDesc_.SampleDesc.Count = 1;								//マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER;				//バックバッファ用
	swapChainDesc_.BufferCount = 2;									//バッファ数を2つに設定
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			//フリップ後は破棄
	swapChainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//スワップチェーンの生成
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get() ,
		winApp_->GetHwnd() ,
		&swapChainDesc_ ,
		nullptr ,
		nullptr ,
		&swapChain1_
	);
	assert(SUCCEEDED(result));

	//生成したIDXGIswapChain1_のオブジェクトをIDXGIswapChain_4に変換する
	swapChain1_.As(&swapChain_);

	//デスクリプタヒープ
	//デスクリプタヒープの設定
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc_.NumDescriptors = swapChainDesc_.BufferCount;

	//デスクリプタヒープの生成
	device_->CreateDescriptorHeap(&rtvHeapDesc_ , IID_PPV_ARGS(&rtvHeap_));

	//バックバッファ
	backBuffers_.resize(swapChainDesc_.BufferCount);

	//レンダービューターゲット
	//スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		//スワップチェーンからバッファを取得
		swapChain_->GetBuffer((UINT)i , IID_PPV_ARGS(&backBuffers_[i]));
		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		//レンダービューターゲットの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//レンダービューターゲットの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get() , &rtvDesc , rtvHandle_);

	}

}

void DX12base::DepthTest() {
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApp_->WINDOW_WIDTH;		//レンダーターゲットに合わせる
	depthResourceDesc.Height = winApp_->WINDOW_WIDTH;	//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	//深度値用ヒーププロパティ
	depthHeapProp_.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	depthClearValue_.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)でクリア
	depthClearValue_.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//リソース生成
	result = device_->CreateCommittedResource(
		&depthHeapProp_ ,
		D3D12_HEAP_FLAG_NONE ,
		&depthResourceDesc ,
		D3D12_RESOURCE_STATE_DEPTH_WRITE ,	//深度値書き込みに使用
		&depthClearValue_ ,
		IID_PPV_ARGS(&depthBuff_)
	);

	//深度ビュー用デスクリプタヒープ
	dsvHeapDesc_.NumDescriptors = 1;	//深度ビューは一つ
	dsvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	result = device_->CreateDescriptorHeap(&dsvHeapDesc_ , IID_PPV_ARGS(&dsvHeap_));

	//深度ビュー作成
	dsvDesc_.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get() ,
		&dsvDesc_ ,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);
}

void DX12base::CreateFence(){

	//フェンスの生成
	result = device_->CreateFence(fenceVal_ , D3D12_FENCE_FLAG_NONE , IID_PPV_ARGS(&fence_));

	assert(SUCCEEDED(result));

}

//アクセッサ
ComPtr<ID3D12GraphicsCommandList> DX12base::GetCmdList() {
	return commandList_.Get();
}

ComPtr<ID3D12Device> DX12base::GetDevice() {
	return device_.Get();
}