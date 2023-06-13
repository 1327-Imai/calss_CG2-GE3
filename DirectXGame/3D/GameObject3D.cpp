#include "GameObject3D.h"

LightGroup* GameObject3D::light = nullptr;



//�����o�֐�
void GameObject3D::PreLoadModel(const char* modelFileName) {
	this->modelFileName = modelFileName;
}

//�����o�֐�
void GameObject3D::PreLoadTexture(const wchar_t* textureFileName) {
	this->textureFileName = textureFileName;
}

void GameObject3D::Initialize() {

	InitializeConstMapTransform();
	InitializeConstMapMaterial();

	//���[���h�ϊ��̏�����
	worldTransform.initialize();

	//���f���̏�����
	model.LoadModel(modelFileName,true);
	model.Initialize();

	//�e�N�X�`���̏�����
	textrue.LoadTexture(textureFileName);
	textrue.SetModel(&model);
	textrue.CreateSRV();
}

void GameObject3D::Update() {

	worldTransform.UpdateMatWorld();

}

void GameObject3D::Draw() {

	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform->world = worldTransform.matWorld_;
	constMapTransform->viewprojection = viewProjection->matView;
	constMapTransform->viewprojection *= *matProjection;
	constMapTransform->cameraPos = viewProjection->GetEye();

	//���_�o�b�t�@�\�r���[���Z�b�g����R�}���h
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial->GetGPUVirtualAddress());

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform->GetGPUVirtualAddress());

	textrue.Draw();

	model.Draw();

	light->Draw(dx12base.GetCmdList());
}

void GameObject3D::InitializeConstMapTransform() {
	HRESULT result;

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	cbTransformHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	cbTransformResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbTransformResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbTransformResourceDesc.Height = 1;
	cbTransformResourceDesc.DepthOrArraySize = 1;
	cbTransformResourceDesc.MipLevels = 1;
	cbTransformResourceDesc.SampleDesc.Count = 1;
	cbTransformResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbTransformHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbTransformResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0 , nullptr , (void**)&constMapTransform); // �}�b�s���O
	assert(SUCCEEDED(result));

}

void GameObject3D::InitializeConstMapMaterial() {

	HRESULT result;

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	cbMaterialHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};
	cbMaterialResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMaterialResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbMaterialResourceDesc.Height = 1;
	cbMaterialResourceDesc.DepthOrArraySize = 1;
	cbMaterialResourceDesc.MipLevels = 1;
	cbMaterialResourceDesc.SampleDesc.Count = 1;
	cbMaterialResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbMaterialHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbMaterialResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0 , nullptr , (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->ambient = Vector3(0.1f , 0.1f , 0.1f);
	constMapMaterial->diffuse = Vector3(0.8f, 0.8f, 0.8f);
	constMapMaterial->specular = Vector3(0.5f, 0.5f, 0.5f);
	constMapMaterial->alpha = 1.0f;
}


//�A�N�Z�b�T
//void GameObject3D::SetViewProjection(ViewProjection* viewProjection) {
//	this->viewProjection = viewProjection;
//}

void GameObject3D::SetMatProjection(Matrix4* matProjection) {
	this->matProjection = matProjection;
}

//���[���h���W���擾
Vector3 GameObject3D::GetWorldPosition()
{
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform.matWorld_.m[3][0];
	worldPos.y = worldTransform.matWorld_.m[3][1];
	worldPos.z = worldTransform.matWorld_.m[3][2];

	return worldPos;
}