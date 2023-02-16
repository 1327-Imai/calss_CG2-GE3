#include "Sprite.h"
//�R���X�g���N�^
Sprite::Sprite() {

}

//�f�X�g���N�^
Sprite::~Sprite() {

}

//�����o�֐�
void Sprite::Initialize(SpriteCommon* spriteCommon , Input* input) {

	HRESULT result;

	input_ = input;

	//DirectX��b
	dx12base_ = spriteCommon->GetDX12Base();

	// ���_�o�b�t�@�r���[
	vbView_ = spriteCommon->GetVBView();

	//���_��
	verticesValue = spriteCommon->GetVerticesValue();

	//SRv�q�[�v
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
	////���_�o�b�t�@�̐ݒ�
	dx12base_->GetCmdList()->IASetVertexBuffers(0 , 1 , &vbView_);

	//�萔�o�b�t�@�\�r���[���Z�b�g����R�}���h
	dx12base_->GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial_->GetGPUVirtualAddress());

	//SRV�q�[�v�̐ݒ�R�}���h
	dx12base_->GetCmdList()->SetDescriptorHeaps(1 , srvHeap_.GetAddressOf());

	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();

	if (input_->PushKey(DIK_SPACE)) {
		//�񖇖ڂ��w�������悤�ɍ�������
		srvGpuHandle.ptr += incremantSize_;
	}

	dx12base_->GetCmdList()->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12base_->GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform_->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12base_->GetCmdList()->DrawInstanced(verticesValue , 1 , 0 , 0);


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
	result = dx12base_->GetDevice()->CreateCommittedResource(
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
	result = dx12base_->GetDevice()->CreateCommittedResource(
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
	constMapMaterial_->color = Vector4(1.0f , 1.0f , 1.0f , 0.5f);

}
