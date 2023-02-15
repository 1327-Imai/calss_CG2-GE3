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

	//�\����
private:
	struct Vertex {
		Vector3 pos;	//xyz���W
	};

	//�����o�ϐ�
private:

	//DirectX��b
	DX12base* dx12base_ = nullptr;

	//���_
	XMFLOAT3 vertices_[3];

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_;

	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
};

