#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Texture.h"

class Object3D
{
public:
	//�R���X�g���N�^
	Object3D();

	//�f�X�g���N�^
	~Object3D();

public://�ÓI�����o�֐�
	static void StaticInitialize(DirectXCommon* dxCommon , ViewProjection* viewProjection);

	static void CreateGraphicsPipeline();

	//�A�N�Z�b�T
	static void SetDevice(ID3D12Device* device) {
		Object3D::device = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Object3D::cmdList = cmdList;
	}
	static void SetViewProjection(ViewProjection* viewProjection) {
		Object3D::viewProjection = viewProjection;
	}

	//�����o�֐�
public:
	//����������
	void Initialize();

	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

	void SetTexture(Texture* texture);

	//�A�N�Z�b�T
	void SetModel(Model* model) {
		this->model = model;
	}

	void SetPosition(Vector3 position) {
		worldTransform.translation_ = position;
	}
	Vector3 GetPosition() const {
		Vector3 pos = {
			worldTransform.matWorld_.m[3][0] ,
			worldTransform.matWorld_.m[3][1] ,
			worldTransform.matWorld_.m[3][2]
		};
		return pos;
	}

	void SetRotation(Vector3 rotation) {
		worldTransform.rotation_ = rotation;
	}
	Vector3 GetRotation() {
		return worldTransform.rotation_;
	}

	void SetScale(Vector3 scale) {
		worldTransform.scale_ = scale;
	}
	Vector3 GetScale() {
		return worldTransform.scale_;
	}


private:
	//�萔�o�b�t�@�̐���
	void CreateConstBufferTransform();
	void CreateConstBufferMaterial();

	//�萔�o�b�t�@�̍X�V
	void UpdateConstBufferTransform();

private:
	//�\����
	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial {
		Vector4 color; //�F(RGBA)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform {
		Matrix4 mat; //3D�ϊ��s��
	};

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	//�p�C�v���C���X�e�[�g
	static ComPtr<ID3D12PipelineState> pipelineState;
	//�r���[�v���W�F�N�V����
	static ViewProjection* viewProjection;

private:
	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBufferTransform = nullptr;
	//�萔�o�b�t�@(�}�e���A��)
	ComPtr<ID3D12Resource> constBufferMaterial = nullptr;

	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRV�q�[�v�̐擪�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	//���[���h�ϊ�
	WorldTransform worldTransform;
	//���f��
	Model* model;
	//�e�N�X�`��
	Texture* texture;

};

