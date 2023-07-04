#pragma once
#include "MathFunc.h"
#include "vector"
#include <array>
#include "DirectXCommon.h"

class SpriteCommon {
public:

	//�R���X�g���N�^
	SpriteCommon();

	//�f�X�g���N�^
	~SpriteCommon();

	//�����o�֐�
	void Initialize(DirectXCommon* dxCommon);

	void PreDraw();

	void Set2DCoordinateSystem(Matrix4& mat);

	UINT GetVerticesValue() {
		return _countof(vertices_);
	}

	D3D12_VERTEX_BUFFER_VIEW* GetVBView() {
		return &vbView_;
	}

	DirectXCommon* GetDxCommon() {
		return dxCommon_;
	}

	ComPtr<ID3D12Resource> GetVertBuff() const {
		return vertBuff_;
	}

	D3D12_RESOURCE_DESC GetResDesc() {
		return resDesc_;
	}

	//�\����
private:
	struct Vertex {
		Vector3 pos;	//xyz���W
		Vector2 uv;		//uv���W
	};

	//�����o�ϐ�
private:

	//DirectX��b
	DirectXCommon* dxCommon_ = nullptr;

	//���_
	Vertex vertices_[4];

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc_{};

	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_;

	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

};