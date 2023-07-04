#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"

class Object3D
{
public:
	//�R���X�g���N�^
	Object3D();

	//�f�X�g���N�^
	~Object3D();

	//�ÓI�����o�֐�
public:
	static void StaticInitialize(DirectXCommon* dxCommon , Camera* camera);

	static void SetCamera(Camera* camera) {
		Object3D::camera_ = camera;
	}

private:
	static void CreateGraphicsPipeline();

	//�A�N�Z�b�T
	static void SetDevice(ID3D12Device* device) {
		Object3D::device_ = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Object3D::cmdList_ = cmdList;
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
		model_ = model;
	}

	void SetPosition(Vector3 position) {
		worldTransform_.translation_ = position;
	}
	Vector3 GetPosition() const {
		Vector3 pos = {
			worldTransform_.matWorld_.m[3][0] ,
			worldTransform_.matWorld_.m[3][1] ,
			worldTransform_.matWorld_.m[3][2]
		};
		return pos;
	}

	void SetRotation(Vector3 rotation) {
		worldTransform_.rotation_ = rotation;
	}
	Vector3 GetRotation() {
		return worldTransform_.rotation_;
	}

	void SetScale(Vector3 scale) {
		worldTransform_.scale_ = scale;
	}
	Vector3 GetScale() {
		return worldTransform_.scale_;
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
	static ID3D12Device* device_;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList_;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature_;
	//�p�C�v���C���X�e�[�g
	static ComPtr<ID3D12PipelineState> pipelineState_;
	//�J����
	static Camera* camera_;


private:
	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBufferTransform_ = nullptr;
	//�萔�o�b�t�@(�}�e���A��)
	ComPtr<ID3D12Resource> constBufferMaterial_ = nullptr;

	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
	//SRV�q�[�v�̐擪�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

	//���[���h�ϊ�
	WorldTransform worldTransform_;
	//���f��
	Model* model_;
	//�e�N�X�`��
	Texture* texture_;

};

