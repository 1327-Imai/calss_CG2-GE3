#pragma once
#include <d3d12.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <DirectXmath.h>
#include <d3dcompiler.h>

#include<wrl.h>

#include "DX12base.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")

#include <string.h>
#include <unordered_map>
using namespace Microsoft::WRL;
using namespace DirectX;

//class DX12base;

class Model{
public:
	//�����o�֐�
	void LoadModel();
	void LoadModel(const char* fileName, bool smoothing = false);

	void Initialize();

	void Draw();


	// �G�b�W�������f�[�^�̒ǉ�
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	// ���������ꂽ���_�@���̌v�Z
	void CalculateSmoothedVertexNormals();
private:


public:
	//�A�N�Z�b�T
	D3D12_RESOURCE_DESC GetResDesc();

	D3D12_VERTEX_BUFFER_VIEW GetVbView();

	D3D12_INDEX_BUFFER_VIEW GetIbView();

	// ���_�f�[�^�̐����擾
	inline size_t GetVertexCount() { return vertices.size(); }
private:
	//�\����
	struct Vertex {
		XMFLOAT3 pos;	//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;	//uv���W
	};

	//�����o�ϐ�
	//result
	HRESULT result;

	//dierectX12�̊�b�ݒ�
	DX12base& dx12base = DX12base::GetInstance();

	//���f���̃t�@�C���`��
	char fileType[5]={0};

	// ���_�f�[�^�z��
	std::vector<Vertex> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB;

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc;

	//���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	//�C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff = nullptr;

	//GPU��̃o�b�t�@�̃}�b�v
	Vertex* vertMap = nullptr;

	//�C���f�b�N�X�o�b�t�@�̃}�b�v
	uint16_t* indexMap = nullptr;

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView;

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView;

	// ���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>>smoothData;
};

