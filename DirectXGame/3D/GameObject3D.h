#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Texture.h"
#include "DX12base.h"

#include "MathFunc.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "LightGroup.h"


class GameObject3D {

public:
	//�����o�ϐ�
	void PreLoadModel(const char* modelFileName);
	void PreLoadTexture(const wchar_t* textureFileName);

	void Initialize();

	void Update();

	void Draw();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�A�N�Z�b�T
	void SetViewProjection(ViewProjection* viewProjection){this->viewProjection=viewProjection;}
	void SetMatProjection(Matrix4* matProjection);

	/// <summary>
	/// ���C�g�̃Z�b�g
	/// </summary>
	static void SetLight(LightGroup* light) { GameObject3D::light = light; }

private:
	void InitializeConstMapTransform();
	void InitializeConstMapMaterial();

	//�\����
private:
	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial {
		Vector3 ambient;  // �A���r�G���g�W��
		float pad1;        // �p�f�B���O
		Vector3 diffuse;  // �f�B�t���[�Y�W��
		float pad2;        // �p�f�B���O
		Vector3 specular; // �X�y�L�����[�W��
		float pad3;        // �p�f�B���O

		float alpha;       // �A���t�@
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform {
		//Matrix4 mat; //3D�ϊ��s��
		Matrix4 viewprojection;// �r���[�v���W�F�N�V�����s��
		Matrix4 world;// ���[���h�s��
		Vector3 cameraPos;// �J�������W�i���[���h���W�j
	};

	//�����o�ϐ�
public:
	//���[���h�ϊ�
	WorldTransform worldTransform;


private:
	//���f��
	Model model;
	//���f���̃t�@�C����
	const char* modelFileName = nullptr;

	//�e�N�X�`��
	Texture textrue;
	//���f���̃t�@�C����
	const wchar_t* textureFileName = nullptr;

	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//�萔�o�b�t�@�}�b�s���O(�s��p)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbTransformHeapProp;
	D3D12_HEAP_PROPERTIES cbMaterialHeapProp{};

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbTransformResourceDesc;
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	Matrix4* matProjection;

	//DirectX��b����
	DX12base& dx12base = DX12base::GetInstance();
	//�r���[�v���W�F�N�V����
	ViewProjection* viewProjection;

	Vector3 ambient;            // �A���r�G���g�e���x
	Vector3 diffuse;            // �f�B�t���[�Y�e���x
	Vector3 specular;           // �X�y�L�����[�e���x
	float alpha;                 // �A���t�@

	static LightGroup* light;
};

