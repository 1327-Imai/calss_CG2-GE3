#pragma once
#include "MathFunc.h"
#include "vector"
#include <array>
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

	D3D12_VERTEX_BUFFER_VIEW* GetVBView();

	DX12base* GetDX12Base();

	void Set2DCoordinateSystem(Matrix4& mat);

	void LoadTexture(uint32_t index , const std::string& fileName);

	void SetTextureCommands(uint32_t index);

	ComPtr<ID3D12Resource> GetTextureBuffer(uint32_t index)const;

	ComPtr<ID3D12Resource> GetVertBuff()const;

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

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc_{};

	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_;

	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

	//SRV�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

	//ComPtr<ID3D12Resource> texBuff_ = nullptr;

	//ComPtr<ID3D12Resource> texBuff2_ = nullptr;

	//SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;
	std::array<ComPtr<ID3D12Resource> , kMaxSRVCount> textureBuffers_;

	static std::string kDefaultTextureDerectoryPath;

	UINT incremantSize_;

};