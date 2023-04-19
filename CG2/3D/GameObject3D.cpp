#include "GameObject3D.h"

LightGroup* GameObject3D::light = nullptr;



//メンバ関数
void GameObject3D::PreLoadModel(const char* modelFileName) {
	this->modelFileName = modelFileName;
}

//メンバ関数
void GameObject3D::PreLoadTexture(const wchar_t* textureFileName) {
	this->textureFileName = textureFileName;
}

void GameObject3D::Initialize() {

	InitializeConstMapTransform();
	InitializeConstMapMaterial();

	//ワールド変換の初期化
	worldTransform.initialize();

	//モデルの初期化
	model.LoadModel(modelFileName,true);
	model.Initialize();

	//テクスチャの初期化
	textrue.LoadTexture(textureFileName);
	textrue.SetModel(&model);
	textrue.CreateSRV();
}

void GameObject3D::Update() {

	worldTransform.UpdateMatWorld();

}

void GameObject3D::Draw() {

	//定数バッファへデータ転送
	constMapTransform->world = worldTransform.matWorld_;
	constMapTransform->viewprojection = viewProjection->matView;
	constMapTransform->viewprojection *= *matProjection;
	constMapTransform->cameraPos = viewProjection->GetEye();

	//頂点バッファ―ビューをセットするコマンド
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial->GetGPUVirtualAddress());

	//定数バッファビュー(CBV)の設定コマンド
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform->GetGPUVirtualAddress());

	textrue.Draw();

	model.Draw();

	light->Draw(dx12base.GetCmdList());
}

void GameObject3D::InitializeConstMapTransform() {
	HRESULT result;

	//定数バッファの生成
	//ヒープ設定
	cbTransformHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	cbTransformResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbTransformResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbTransformResourceDesc.Height = 1;
	cbTransformResourceDesc.DepthOrArraySize = 1;
	cbTransformResourceDesc.MipLevels = 1;
	cbTransformResourceDesc.SampleDesc.Count = 1;
	cbTransformResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbTransformHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbTransformResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform->Map(0 , nullptr , (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));

}

void GameObject3D::InitializeConstMapMaterial() {

	HRESULT result;

	//定数バッファの生成
	//ヒープ設定
	cbMaterialHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};
	cbMaterialResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMaterialResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbMaterialResourceDesc.Height = 1;
	cbMaterialResourceDesc.DepthOrArraySize = 1;
	cbMaterialResourceDesc.MipLevels = 1;
	cbMaterialResourceDesc.SampleDesc.Count = 1;
	cbMaterialResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbMaterialHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbMaterialResourceDesc ,	//リソース設定
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
	constMapMaterial->ambient = Vector3(0.1f , 0.1f , 0.1f);
	constMapMaterial->diffuse = Vector3(0.8f, 0.8f, 0.8f);
	constMapMaterial->specular = Vector3(0.5f, 0.5f, 0.5f);
	constMapMaterial->alpha = 1.0f;
}


//アクセッサ
//void GameObject3D::SetViewProjection(ViewProjection* viewProjection) {
//	this->viewProjection = viewProjection;
//}

void GameObject3D::SetMatProjection(Matrix4* matProjection) {
	this->matProjection = matProjection;
}

//ワールド座標を取得
Vector3 GameObject3D::GetWorldPosition()
{
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform.matWorld_.m[3][0];
	worldPos.y = worldTransform.matWorld_.m[3][1];
	worldPos.z = worldTransform.matWorld_.m[3][2];

	return worldPos;
}