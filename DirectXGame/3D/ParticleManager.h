#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "Camera.h"

#include <forward_list>

class ParticleManager
{
public:
	//�R���X�g���N�^
	ParticleManager();

	//�f�X�g���N�^
	~ParticleManager();

	//�\����
private:
	struct VertexPos {
		Vector3 pos;	//xyz���W
		float scale;
	};

	struct Particle {

		Vector3 pos;		//���W
		Vector3 velocity;	//���x
		Vector3 accel;		//�����x
		int flame = 0;		//���݃t���[��
		int eFlame = 0;		//�I���t���[��
		float scale = 1.0f;	//���݃X�P�[��
		float sScale = 1.0f;//�����X�P�[��
		float eScale = 0.0f;//�ŏI�X�P�[��

	};

	//�ÓI�����o�֐�
public:
	static void StaticInitialize(DirectXCommon* dxCommon , Camera* camera);

	static void SetCamera(Camera* camera) {
		ParticleManager::camera_ = camera;
	}

private:
	static void CreateGraphicsPipeline();

	static void CreateModel();

	//�A�N�Z�b�T
	static void SetDevice(ID3D12Device* device) {
		ParticleManager::device_ = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		ParticleManager::cmdList_ = cmdList;
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

	void Add(int life , Vector3 position , Vector3 velocity , Vector3 accel,float startSclae, float endScale);

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
		Matrix4 matBillboard;
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
	static const int vertexCount_ = 1024;
	//���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
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

	std::forward_list<Particle> particles;

	//�e�N�X�`��
	Texture* texture_;

};

