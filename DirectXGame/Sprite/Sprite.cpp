#include "Sprite.h"
//�R���X�g���N�^
Sprite::Sprite() {

}

//�f�X�g���N�^
Sprite::~Sprite() {

}

//�����o�֐�
void Sprite::Initialize(SpriteCommon* spriteCommon , Texture* texture) {

	//�X�v���C�g���ʏ���
	spriteCommon_ = spriteCommon;

	//DirectX��b
	dxCommon_ = spriteCommon_->GetDxCommon();

	HRESULT result;

	vertices_[0] = {{0.0f , 100.0f , 0.0f} , {0.0f , 1.0f}}; //����
	vertices_[1] = {{0.0f , 0.0f , 0.0f} , {0.0f , 0.0f}}; //����
	vertices_[2] = {{100.0f , 100.0f , 0.0f} , {1.0f , 1.0f}}; //�E��
	vertices_[3] = {{100.0f , 0.0f , 0.0f} , {1.0f , 0.0f}}; //�E��

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = spriteCommon_->GetVertBuff()->Map(0 , nullptr , (void**)&vertMap_);
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap_[i] = vertices_[i];	//���W���R�s�[
	}
	//�q���������
	spriteCommon_->GetVertBuff()->Unmap(0 , nullptr);
	
	if (texture) {
		texture_ = texture;

		texture_->SetSRV(srvHeap , srvHandle , spriteCommon_->GetResDesc());

		AdujustTextureSize();
		size_ = textureSize_;
	}

	//���[���h�ϊ�
	worldTransform_.initialize();
	//2D���W�n��K�p
	spriteCommon_->Set2DCoordinateSystem(worldTransform_.matWorld_);

	//2D���W
	position_ = {0.0f , 0.0f};
	//��]
	rotation_ = 0;
	//�\���T�C�Y
	size_ = {100 , 100};
	//�X�P�[��
	scale_ = {1.0f , 1.0f};

	//�F
	color_ = {1.0f , 1.0f , 1.0f , 0.5f};

	//�A���J�[
	anchorPoint_ = {0.5 , 0.5};

	//�萔�o�b�t�@�̐���
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

	if (texture_) {

		float tex_left = textureleftTop_.x / texture_->GetResDesc().Width;
		float tex_right = textureleftTop_.x + textureSize_.x / texture_->GetResDesc().Width;
		float tex_top = textureleftTop_.y / texture_->GetResDesc().Height;
		float tex_bottom = textureleftTop_.y + textureSize_.y / texture_->GetResDesc().Height;

		vertices_[LB].uv = {tex_left , tex_bottom};
		vertices_[LT].uv = {tex_left , tex_top};
		vertices_[RB].uv = {tex_right , tex_bottom};
		vertices_[RT].uv = {tex_right , tex_top};
	}

	std::copy(std::begin(vertices_) , std::end(vertices_) , vertMap_);

	//2D���W�A��]�A�X�P�[�������[���h�ϊ��ɑ��
	worldTransform_.translation_ = Vector3{position_.x - anchorPoint_.x * -100 , position_.y - anchorPoint_.y * -100 , 0.0f};
	worldTransform_.rotation_ = Vector3{0.0f , 0.0f , rotation_};
	worldTransform_.scale_ = Vector3{scale_.x , scale_.y , 1.0f};

	//X���t���b�v(true�Ȃ�t���b�v����)
	if (isFlipX_ == true) {
		worldTransform_.scale_.x *= -1;
	}

	//Y���t���b�v(true�Ȃ�t���b�v����)
	if (isFlipY_ == true) {
		worldTransform_.scale_.y *= -1;
	}

	//�s��̍X�V
	worldTransform_.UpdateMatWorld();

	//2D���W�n��K�p
	spriteCommon_->Set2DCoordinateSystem(worldTransform_.matWorld_);

	//�萔�o�b�t�@�֓]��
	constMapTransform_->mat = worldTransform_.matWorld_;
	constMapMaterial_->color = color_;

}

void Sprite::Draw() {
	//�\���t���O���`�F�b�N(false�Ȃ��\��)
	if (isVisible_ == false) {
		return;
	}
	////���_�o�b�t�@�̐ݒ�
	dxCommon_->GetCmdList()->IASetVertexBuffers(0 , 1 , spriteCommon_->GetVBView());

	//�萔�o�b�t�@�\�r���[���Z�b�g����R�}���h
	dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial_->GetGPUVirtualAddress());

	//spriteCommon_->SetTextureCommands(textureIndex_);

	texture_->Draw(srvHeap);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform_->GetGPUVirtualAddress());

	//�`��R�}���h
	dxCommon_->GetCmdList()->DrawInstanced(spriteCommon_->GetVerticesValue() , 1 , 0 , 0);
}

void Sprite::CreateConstMapTransform() {
	HRESULT result;

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform_)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform_->Map(0 , nullptr , (void**)&constMapTransform_); // �}�b�s���O
	assert(SUCCEEDED(result));

	constMapTransform_->mat = worldTransform_.matWorld_;
}

void Sprite::CreateConstMapMaterial() {

	HRESULT result;

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffMaterial_)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial_->Map(0 , nullptr , (void**)&constMapMaterial_); // �}�b�s���O
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial_->color = color_;

}

void Sprite::SetTexture(Texture* texture) {
	texture_ = texture;
	texture_->SetSRV(srvHeap , srvHandle , spriteCommon_->GetResDesc());
	AdujustTextureSize();
}

void Sprite::AdujustTextureSize() {

	textureSize_.x = static_cast<float>(texture_->GetResDesc().Width);
	textureSize_.y = static_cast<float>(texture_->GetResDesc().Height);
}