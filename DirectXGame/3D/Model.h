#pragma once
#include "DirectXCommon.h"
#include "MathFunc.h"
#include <fstream>
#include <sstream>
#include <string>

class Model
{
public:
	//�R���X�g���N�^
	Model();

	//�f�X�g���N�^
	~Model();
private://�G�C���A�X
	//std���ȗ�
	using string = std::string;

public://�萔
	//���f���i�[���[�g�p�X
	static const string baseDirectory;

public:
	//�ÓI�����o�֐�
	//�A�N�Z�b�T
	static void SetDevice(ID3D12Device* device) {
		Model::device = device;
	}
	static void SetCmdList(ID3D12GraphicsCommandList* cmdList) {
		Model::cmdList = cmdList;
	}

	//�����o�֐�
public:
	//������
	void LoadModel(const string& modelName);

	//�`��
	void Draw();

	//���f�����擾
	string GetModelName() {
		return modelName;
	}

	D3D12_RESOURCE_DESC GetResDesc() {
		return resDesc;
	}

	//�\����
private:
	struct VertexPosNormalUv {
		Vector3 pos;	//xyz���W
		Vector3 normal;	//�@���x�N�g��
		Vector2 uv;	//uv���W
	};

	//�ÓI�����o�ϐ�
private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;

	//�����o�ϐ�
private:
	//���_
	std::vector<VertexPosNormalUv> vertices;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//�C���f�b�N�X
	std::vector<uint16_t> indices;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc;

	//���f����
	string modelName;

};

