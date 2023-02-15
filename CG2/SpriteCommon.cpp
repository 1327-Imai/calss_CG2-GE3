#include "SpriteCommon.h"

//コンストラクタ
SpriteCommon::SpriteCommon() {

}

//デストラクタ
SpriteCommon::~SpriteCommon() {

}

//メンバ関数
void SpriteCommon::Initialize(DX12base dx12base) {

	HRESULT result;

	vertices_ = {
		{{-5.0f , -5.0f , -5.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{-5.0f , 5.0f , -5.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{5.0f , -5.0f , -5.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{5.0f , 5.0f , -5.0f} , {} , {1.0f , 0.0f}} //右上 インデックス3
	};

	//インデックスデータ
	indices_ = {
		0 , 1 , 2 , //三角形1つ目
		1 , 3 , 2 , //三角形2つ目
	};

	//法線の計算
	for (int i = 0; i < indices_.size() / 3; i++) {

		//三角形のインデックスを取り出して一時的な変数に入れる
		unsigned short index0 = indices_[i * 3 + 0];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		Vector3 p0 = vertices_[index0].pos;
		Vector3 p1 = vertices_[index1].pos;
		Vector3 p2 = vertices_[index2].pos;

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;

		//外積は両方から垂直なベクトル
		Vector3 normal = v1.cross(v2);

		//正規化
		normal.nomalize();

		//求めた法線を頂点データに代入
		vertices_[index0].normal = normal;
		vertices_[index1].normal = normal;
		vertices_[index2].normal = normal;

	}


	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB_ = static_cast<UINT>(sizeof(Vertex) * vertices_.size());

	//インデックスデータ全体のサイズ
	sizeIB_ = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB_; //頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc_ ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff_)
	);
	assert(SUCCEEDED(result));

	//インデックスバッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc_ ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&indexBuff_)
	);
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0 , nullptr , (void**)&vertMap_);
	//全頂点に対して
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i];	//座標をコピー
	}
	//繋がりを解除
	vertBuff_->Unmap(0 , nullptr);

	//インデックスバッファをマッピング
	result = indexBuff_->Map(0 , nullptr , (void**)&indexMap_);
	//全インデックスに対して
	for (int i = 0; i < indices_.size(); i++) {
		indexMap_[i] = indices_[i];	//座標をコピー
	}
	//繋がりを解除
	indexBuff_->Unmap(0 , nullptr);

	//GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB_;
	//頂点１つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	//インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB_;

}

void SpriteCommon::Update() {

}

void SpriteCommon::Draw() {

}