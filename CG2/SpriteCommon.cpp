#include "SpriteCommon.h"

//コンストラクタ
SpriteCommon::SpriteCommon() {

}

//デストラクタ
SpriteCommon::~SpriteCommon() {

}

//メンバ関数
void SpriteCommon::Initialize(DX12base* dx12base) {
	
	dx12base_ = dx12base;

	HRESULT result;
#pragma region//描画初期化処理

	vertices_[0] = {-0.5f , -0.5f , 0.0f}; //左下
	vertices_[1] = {-0.5f , +0.5f , 0.0f}; //左上
	vertices_[2] = {+0.5f , -0.5f , 0.0f}; //右上

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices_));

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

	//頂点バッファの設定
	//ComPtr<ID3D12Resource> vertBuff = nullptr;
	result = dx12base_->GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0 , nullptr , (void**)&vertMap);
	//全頂点に対して
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i];	//座標をコピー
	}
	//繋がりを解除
	vertBuff->Unmap(0 , nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点１つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(XMFLOAT3);

	//頂点シェーダーファイルの読み込みとコンパイル
	ID3DBlob* vsBlob = nullptr;		//頂点シェーダーオブジェクト
	ID3DBlob* psBlob = nullptr;		//ピクセルシェーダーオブジェクト
	ID3DBlob* errorBlob = nullptr;	//エラーオブジェクト

	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/Sprite/SpriteVS.hlsl" ,									//シェーダーファイル名
		nullptr ,
		D3D_COMPILE_STANDARD_FILE_INCLUDE ,					//インクルード可能にする
		"main" ,												//エントリーポイント名
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

	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/Sprite/SpritePS.hlsl" ,									//シェーダーファイル名
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

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
	};

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定

	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に

	////ブレンドステート
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

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	pipelineDesc.NumRenderTargets = 1;								//描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1;								//1ピクセルにつき1回のサンプリング


	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc , D3D_ROOT_SIGNATURE_VERSION_1_0 ,
										 &rootSigBlob , &errorBlob);
	assert(SUCCEEDED(result));

	result = dx12base_->GetDevice()->CreateRootSignature(0 , rootSigBlob->GetBufferPointer() , rootSigBlob->GetBufferSize() ,
														 IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature_.Get();

	//パイプラインステートの生成
	result = dx12base_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc , IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));

}

void SpriteCommon::PreDraw() {

	//パイプラインステートとルートシグネチャの設定コマンド
	dx12base_->GetCmdList()->SetPipelineState(pipelineState_.Get());
	dx12base_->GetCmdList()->SetGraphicsRootSignature(rootSignature_.Get());

	// プリミティブ形状の設定コマンド
	dx12base_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

UINT SpriteCommon::GetVerticesValue() {
	return _countof(vertices_);
}

D3D12_VERTEX_BUFFER_VIEW SpriteCommon::GetVBView() {
	return vbView_;
}

DX12base* SpriteCommon::GetDX12Base() {
	return dx12base_;
}