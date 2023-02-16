#pragma once
#include "MathFunc.h"
#include "vector"
#include "DX12base.h"

class SpriteCommon {
public:

	//�R���X�g���N�^
	SpriteCommon();

	//�f�X�g���N�^
	~SpriteCommon();

	//�����o�֐�
	void Initialize(DX12base* dx12base);

	void PreDraw();

	UINT GetVerticesValue();

	D3D12_VERTEX_BUFFER_VIEW GetVBView();

	DX12base* GetDX12Base();

	ComPtr<ID3D12DescriptorHeap> GetSrvHeap();

	UINT GetIncemantSize();

	void Set2DCoordinateSystem(Matrix4& mat);

	//�\����
private:
	struct Vertex {
		Vector3 pos;	//xyz���W
		Vector2 uv;		//uv���W
	};

	//�����o�ϐ�
private:

	//DirectX��b
	DX12base* dx12base_ = nullptr;

	//���_
	Vertex vertices_[4];

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_;

	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

	//SRV�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	ComPtr<ID3D12Resource> texBuff_ = nullptr;

	ComPtr<ID3D12Resource> texBuff2_ = nullptr;

	UINT incremantSize_;

	Vector2 position_;




};