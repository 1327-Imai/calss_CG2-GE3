#include "Sprite.h"
//コンストラクタ
Sprite::Sprite() {

}

//デストラクタ
Sprite::~Sprite() {

}

//メンバ関数
void Sprite::Initialize(SpriteCommon* spriteCommon , Input* input) {

	HRESULT result;

	input_ = input;

	//DirectX基礎
	dx12base_ = spriteCommon->GetDX12Base();

	// 頂点バッファビュー
	vbView_ = spriteCommon->GetVBView();

	//頂点数
	verticesValue = spriteCommon->GetVerticesValue();

	//SRvヒープ
	srvHeap_ = spriteCommon->GetSrvHeap();

	incremantSize_ = spriteCommon->GetIncemantSize();

	CreateConstMapTransform();

	CreateConstMapMaterial();

	mat.SetIdentityMatrix();

	spriteCommon->Set2DCoordinateSystem(mat);

	constMapTransform_->mat = mat;

	position_ = {0.0f , 0.0f};
}

void Sprite::Update() {


}

void Sprite::Draw() {
	////頂点バッファの設定
	dx12base_->GetCmdList()->IASetVertexBuffers(0 , 1 , &vbView_);

	//定数バッファ―ビューをセットするコマンド
	dx12base_->GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial_->GetGPUVirtualAddress());

	//SRVヒープの設定コマンド
	dx12base_->GetCmdList()->SetDescriptorHeaps(1 , srvHeap_.GetAddressOf());

	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();

	if (input_->PushKey(DIK_SPACE)) {
		//二枚目を指し示すように差し込む
		srvGpuHandle.ptr += incremantSize_;
	}

	dx12base_->GetCmdList()->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);

	//定数バッファビュー(CBV)の設定コマンド
	dx12base_->GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform_->GetGPUVirtualAddress());

	//描画コマンド
	dx12base_->GetCmdList()->DrawInstanced(verticesValue , 1 , 0 , 0);


}

void Sprite::CreateConstMapTransform() {
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

	//定数バッファの生成
	result = dx12base_->GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform_)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform_->Map(0 , nullptr , (void**)&constMapTransform_); // マッピング
	assert(SUCCEEDED(result));

}

void Sprite::CreateConstMapMaterial() {

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

	//定数バッファの生成
	result = dx12base_->GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffMaterial_)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffMaterial_->Map(0 , nullptr , (void**)&constMapMaterial_); // マッピング
	assert(SUCCEEDED(result));

	//定数バッファへのデータ転送
	//値を書き込むと自動的に転送される
	constMapMaterial_->color = Vector4(1.0f , 1.0f , 1.0f , 0.5f);

}
