#include "Sprite.h"
//コンストラクタ
Sprite::Sprite() {

}

//デストラクタ
Sprite::~Sprite() {

}

//メンバ関数
void Sprite::Initialize(SpriteCommon* spriteCommon , uint32_t textureIndex) {

	//スプライト共通処理
	spriteCommon_ = spriteCommon;

	//DirectX基礎
	dx12base_ = spriteCommon_->GetDX12Base();

	HRESULT result;

	vertices_[0] = {{0.0f , 100.0f , 0.0f} , {0.0f , 1.0f}}; //左下
	vertices_[1] = {{0.0f , 0.0f , 0.0f} , {0.0f , 0.0f}}; //左上
	vertices_[2] = {{100.0f , 100.0f , 0.0f} , {1.0f , 1.0f}}; //右上
	vertices_[3] = {{100.0f , 0.0f , 0.0f} , {1.0f , 0.0f}}; //右下

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = spriteCommon_->GetVertBuff()->Map(0 , nullptr , (void**)&vertMap_);
	//全頂点に対して
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap_[i] = vertices_[i];	//座標をコピー
	}
	//繋がりを解除
	spriteCommon_->GetVertBuff()->Unmap(0 , nullptr);

	if (textureIndex != UINT32_MAX) {
		textureIndex_ = textureIndex;
		AdujustTextureSize();

		size_ = textureSize_;
	}
	//ワールド変換
	worldTransform_.initialize();
	//2D座標系を適用
	spriteCommon_->Set2DCoordinateSystem(worldTransform_.matWorld_);

	//2D座標
	position_ = {0.0f , 0.0f};
	//回転
	rotation_ = 0;
	//表示サイズ
	size_ = {100 , 100};
	//スケール
	scale_ = {1.0f , 1.0f};

	//色
	color_ = {1.0f , 1.0f , 1.0f , 0.5f};

	//アンカー
	anchorPoint_ = {0.5 , 0.5};

	//定数バッファの生成
	CreateConstMapTransform();
	CreateConstMapMaterial();
}

void Sprite::Update() {

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	vertices_[LB].pos = {left , bottom , 0.0f};
	vertices_[RB].pos = {right , bottom , 0.0f};
	vertices_[LT].pos = {left , top , 0.0f};
	vertices_[RT].pos = {right , top , 0.0f};

	ComPtr<ID3D12Resource> textureBuffer = spriteCommon_->GetTextureBuffer(textureIndex_);

	if (textureBuffer) {
		D3D12_RESOURCE_DESC resDesc = textureBuffer.Get()->GetDesc();

		float tex_left = textureleftTop_.x / resDesc.Width;
		float tex_right = textureleftTop_.x + textureSize_.x / resDesc.Width;
		float tex_top = textureleftTop_.y / resDesc.Height;
		float tex_bottom = textureleftTop_.y + textureSize_.y / resDesc.Height;

		vertices_[LB].uv = {tex_left , tex_bottom};
		vertices_[LT].uv = {tex_left , tex_top};
		vertices_[RB].uv = {tex_right , tex_bottom};
		vertices_[RT].uv = {tex_right , tex_top};
	}

	std::copy(std::begin(vertices_) , std::end(vertices_) , vertMap_);

	//2D座標、回転、スケールをワールド変換に代入
	worldTransform_.translation_ = Vector3{position_.x - anchorPoint_.x * -100 , position_.y - anchorPoint_.y * -100 , 0.0f};
	worldTransform_.rotation_ = Vector3{0.0f , 0.0f , rotation_};
	worldTransform_.scale_ = Vector3{scale_.x , scale_.y , 1.0f};

	//X軸フリップ(trueならフリップする)
	if (isFlipX_ == true) {
		worldTransform_.scale_.x *= -1;
	}

	//Y軸フリップ(trueならフリップする)
	if (isFlipY_ == true) {
		worldTransform_.scale_.y *= -1;
	}

	//行列の更新
	worldTransform_.UpdateMatWorld();

	//2D座標系を適用
	spriteCommon_->Set2DCoordinateSystem(worldTransform_.matWorld_);

	//定数バッファへ転送
	constMapTransform_->mat = worldTransform_.matWorld_;
	constMapMaterial_->color = color_;

}

void Sprite::Draw() {
	//表示フラグをチェック(falseなら非表示)
	if (isVisible_ == false) {
		return;
	}
	////頂点バッファの設定
	dx12base_->GetCmdList()->IASetVertexBuffers(0 , 1 , spriteCommon_->GetVBView());

	//定数バッファ―ビューをセットするコマンド
	dx12base_->GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial_->GetGPUVirtualAddress());

	spriteCommon_->SetTextureCommands(textureIndex_);

	//定数バッファビュー(CBV)の設定コマンド
	dx12base_->GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform_->GetGPUVirtualAddress());

	//描画コマンド
	dx12base_->GetCmdList()->DrawInstanced(spriteCommon_->GetVerticesValue() , 1 , 0 , 0);
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

	constMapTransform_->mat = worldTransform_.matWorld_;
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
	constMapMaterial_->color = color_;

}

void Sprite::SetPosition(const Vector2 position) {
	position_ = position;
}
Vector2 Sprite::GetPosition() const {
	return position_;
}

void Sprite::SetRotation(float rotation) {
	rotation_ = rotation;
}
float Sprite::GetRotation() const {
	return rotation_;
}

void Sprite::SetScale(const Vector2 scale) {
	scale_ = scale;
}
Vector2 Sprite::GetScale() const {
	return scale_;
}

void Sprite::SetSize(const Vector2 size) {
	size_ = size;
}
Vector2 Sprite::GetSize() const {
	return size_;
}

void Sprite::SetAnchorPoint(const Vector2 anchorPoint) {
	anchorPoint_ = anchorPoint;
}
Vector2 Sprite::GetAnchorPoint() const {
	return anchorPoint_;
}

void Sprite::SetColor(Vector4 color) {
	color_ = color;
}
Vector4 Sprite::GetColor() const {
	return color_;
}

void Sprite::SetIsFlipX(bool isFlipX) {
	isFlipX_ = isFlipX;
}
bool Sprite::GetIsFlipX() const {
	return isFlipX_;
}

void Sprite::SetIsFlipY(bool isFlipY) {
	isFlipY_ = isFlipY;
}
bool Sprite::GetIsFlipY() const {
	return isFlipY_;
}

void Sprite::SetIsVisible(bool isVisible) {
	isVisible_ = isVisible;
}
bool Sprite::GetIsVisible() const {
	return isVisible_;
}

void Sprite::SetTextureIndex(uint32_t textureIndex) {
	textureIndex_ = textureIndex;
}
uint32_t Sprite::GetTextureIndex() {
	return textureIndex_;
}

void Sprite::setTextureSize(Vector2 textureSize) {
	textureSize_ = textureSize;
}
Vector2 Sprite::GetTextureSize() {
	return textureSize_;
}

void Sprite::AdujustTextureSize() {
	ComPtr<ID3D12Resource> textureBuffer = spriteCommon_->GetTextureBuffer(textureIndex_);
	assert(textureBuffer);

	D3D12_RESOURCE_DESC resDesc = textureBuffer.Get()->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}