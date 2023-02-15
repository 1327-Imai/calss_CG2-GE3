#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXmath.h>
#include <wrl.h>
#include <chrono>
#include <thread>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#include <cassert>
#include <vector>
#include <string>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class DX12base{

public:
	//�����o�֐�
	//����������
	void Initialize(WinApp* winApp);

	void PreDraw();

	void PostDraw();

private:

	void CreateCmdList();

	void SetSwapChain();

	void DepthTest();

	void CreateFence();

	void InitializeFixFPS();

	void UpdateFixFPS();

public:
	//�A�N�Z�b�T
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();

	ComPtr<ID3D12Device> GetDevice();

	//�����o�ϐ�
private:
	HRESULT result;

	//DirectX����������
	ComPtr<ID3D12Debug> debugController_;
	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;

	//�A�_�v�^�[�̗񋓗�
	std::vector<ComPtr<IDXGIAdapter4>> adapters_;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter_ = nullptr;

	D3D_FEATURE_LEVEL featureLevel_;

	//�R�}���h�L���[
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

#pragma region//�X���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	
	//IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapChain1_;

	//�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};

	//�o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

#pragma endregion

#pragma region//�[�x�e�X�g
	//�f�X�N���v�^
	D3D12_RESOURCE_DESC depthResourceDesc_{};

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp_{};

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue_{};

	//���\�[�X
	ComPtr<ID3D12Resource> depthBuff_ = nullptr;

	//�[�x�r���[�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc_{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_ = {};

#pragma endregion

#pragma region//�t�F���X

	//�t�F���X�̐���
	ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64 fenceVal_ = 0;

#pragma endregion

#pragma region//�`�揈��

	//�o�b�N�o�b�t�@
	UINT bbIndex_;

	//���\�[�X�o���A
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	//�����_�[�^�[�Q�b�g�r���[�̃n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

	//�N���A�J���[
	FLOAT clearColor_[4] = {0.1f , 0.25f , 0.5f , 0.0f};;

	//�r���[�|�[�g
	D3D12_VIEWPORT viewport_{};

	//�V�U�[��`
	D3D12_RECT scissorRect_{};

#pragma endregion

	//�L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;

	//WinApp�̃|�C���^
	WinApp* winApp_ = nullptr;
};

