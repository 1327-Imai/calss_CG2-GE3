#include "SpriteCommon.h"

std::string SpriteCommon::kDefaultTextureDerectoryPath = "Resources/texture/";

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

	vertices_[0] = {{0.0f , 100.0f , 0.0f} , {0.0f , 1.0f}}; //左下
	vertices_[1] = {{0.0f , 0.0f , 0.0f} , {0.0f , 0.0f}}; //左上
	vertices_[2] = {{100.0f , 100.0f , 0.0f} , {1.0f , 1.0f}}; //右上
	vertices_[3] = {{100.0f , 0.0f , 0.0f} , {1.0f , 0.0f}}; //右下

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; //頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの設定
	result = dx12base_->GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc_ ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff_)
	);
	assert(SUCCEEDED(result));

	////GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	//Vertex* vertMap = nullptr;
	//result = vertBuff_->Map(0 , nullptr , (void**)&vertMap);
	////全頂点に対して
	//for (int i = 0; i < _countof(vertices_); i++) {
	//	vertMap[i] = vertices_[i];	//座標をコピー
	//}
	////繋がりを解除
	//vertBuff_->Unmap(0 , nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点１つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

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
		{//xyz座標
			"POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,

		{//uv座標
			"TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 ,
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

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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
	//定数バッファ0番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える

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

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;					//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

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

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	result = dx12base->GetDevice()->CreateDescriptorHeap(
		&srvHeapDesc ,
		IID_PPV_ARGS(&srvHeap_)
	);
	assert(SUCCEEDED(result));

	//デスクリプタハンドル
	//SRVヒープの先頭ハンドルを取得
	srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();

}

void SpriteCommon::PreDraw() {

	//パイプラインステートとルートシグネチャの設定コマンド
	dx12base_->GetCmdList()->SetPipelineState(pipelineState_.Get());
	dx12base_->GetCmdList()->SetGraphicsRootSignature(rootSignature_.Get());

	// プリミティブ形状の設定コマンド
	dx12base_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
}

UINT SpriteCommon::GetVerticesValue() {
	return _countof(vertices_);
}

D3D12_VERTEX_BUFFER_VIEW* SpriteCommon::GetVBView() {
	return &vbView_;
}

DX12base* SpriteCommon::GetDX12Base() {
	return dx12base_;
}

void SpriteCommon::Set2DCoordinateSystem(Matrix4& mat) {

	Matrix4 m;

	m.m[3][0] -= 1.0f;
	m.m[3][1] += 1.0f;
	m.m[0][0] *= 2.0f / WinApp::WINDOW_WIDTH;
	m.m[1][1] *= -2.0f / WinApp::WINDOW_HEIGHT;

	mat *= m;
}

void SpriteCommon::LoadTexture(uint32_t index , const std::string& fileName) {

	HRESULT result;

	std::string fullPath = kDefaultTextureDerectoryPath + fileName;

	int filePathBufferSize = MultiByteToWideChar(
		CP_ACP , 0 , fullPath.c_str() , -1 , nullptr , 0
	);

	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(
		CP_ACP , 0 , fullPath.c_str() , -1 , wfilePath.data() , filePathBufferSize
	);

	//画像イメージデータの作成
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		wfilePath.data() ,
		WIC_FLAGS_NONE ,
		&metadata ,
		scratchImg
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

	//テクスチャバッファの生成
	result = dx12base_->GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&textureBuffers_[index])
	);
	assert(SUCCEEDED(result));


	//テクスチャバッファにデータ転送
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i , 0 , 0);
		//テクスチャバッファにデータ転送
		result = textureBuffers_[index]->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//全領域へコピー
			img->pixels ,			//元データアドレス
			(UINT)img->rowPitch ,	//1ラインサイズ
			(UINT)img->slicePitch	//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	//シェーダーリソースビューの作成
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = resDesc_.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc_.MipLevels;

	incremantSize_ = dx12base_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvHandle_.ptr += incremantSize_ * index;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dx12base_->GetDevice()->CreateShaderResourceView(textureBuffers_[index].Get() , &srvDesc , srvHandle_);
}

void SpriteCommon::SetTextureCommands(uint32_t index) {
	//SRVヒープの設定コマンド
	dx12base_->GetCmdList()->SetDescriptorHeaps(1 , srvHeap_.GetAddressOf());

	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();

	srvGpuHandle.ptr += incremantSize_ * index;

	dx12base_->GetCmdList()->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);
}

ComPtr<ID3D12Resource> SpriteCommon::GetTextureBuffer(uint32_t index)const {
	return textureBuffers_[index];
}

ComPtr<ID3D12Resource> SpriteCommon::GetVertBuff() const{
	return vertBuff_;
}