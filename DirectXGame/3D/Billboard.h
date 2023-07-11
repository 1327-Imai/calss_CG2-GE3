#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "Camera.h"

class Billboard
{
public:
	//�R���X�g���N�^
	Billboard();

	//�f�X�g���N�^
	~Billboard();

	//�\����
private:
	struct VertexPosNormalUv {
		Vector3 pos;	//xyz���W
		Vector3 normal;	//�@���x�N�g��
		Vector2 uv;	//uv���W
	};

	//�ÓI�����o�֐�
public:
	static void StaticInitialize(DirectXCommon* dxCommon , Camera* camera);

	static void SetCamera(Camera* camera) {
		Billboard::camera_ = camera;
	}

private:
	static void CreateGraphicsPipeline();

	static void CreateModel();

	//�A�N�Z�b�T
	static void SetDevice(ID3D12Device* device) {
		Billboard::device_ = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Billboard::cmdList_ = cmdList;
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
	//���_��
	static const int vertexCount_ = 4;
	//���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//���_�z��
	static VertexPosNormalUv vertices[vertexCount_];
	//�C���f�b�N�X��
	static const int indexCount_ = 3 * 2;
	//�C���f�b�N�X�z��
	static uint16_t indices[indexCount_];
	//�C���f�b�N�X�o�b�t�@
	static ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	//���\�[�X�ݒ�
	static D3D12_RESOURCE_DESC resDesc;

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
	//�e�N�X�`��
	Texture* texture_;

};

