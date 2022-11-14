#include "DX12base.h"

void DX12base::Initialize(WinApp* winApp) {
	
	//nullptr�`�F�b�N
	assert(winApp);

	winApp_ = winApp;

	//�f�o�b�O���C���[�̗L����

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		debugController_->EnableDebugLayer();
	}
#endif

	//DirectX����������

	//�A�_�v�^�̗�
	//DX6I�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	//�p�t�H�[�}���X���������̂����珇�ɁA���ׂẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		 dxgiFactory_->EnumAdapterByGpuPreference(i ,
		 DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE ,
		 IID_PPV_ARGS(&tmpAdapter_)) != DXGI_ERROR_NOT_FOUND;
		 i++) {
		//���I�z��ɂ�����
		adapters_.push_back(tmpAdapter_);
	}

	//�Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters_.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//�A�_�v�^�[�̏����擾����
		adapters_[i]->GetDesc3(&adapterDesc);

		//�\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//�f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter_ = adapters_[i];
			break;
		}
	}

	//�f�o�C�X�̐���
	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1 ,
		D3D_FEATURE_LEVEL_12_0 ,
		D3D_FEATURE_LEVEL_11_1 ,
		D3D_FEATURE_LEVEL_11_0 ,
	};

	for (size_t i = 0; i < _countof(levels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(
			tmpAdapter_.Get() ,
			levels[i] ,
			IID_PPV_ARGS(&device_)
		);
		if (result == S_OK) {
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel_ = levels[i];
			break;
		}
	}

	CreateCmdList();
	SetSwapChain();
	DepthTest();
	CreateFence();
}

void DX12base::PreDraw() {
	//�o�b�N�o�b�t�@�̔ԍ����擾
	bbIndex_ = swapChain_->GetCurrentBackBufferIndex();

	//1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc_.Transition.pResource = backBuffers_[bbIndex_].Get();			//�o�b�N�o�b�t�@���w��
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		//�\����Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ�
	commandList_->ResourceBarrier(1 , &barrierDesc_);

	//2.�`���̕ύX
	//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle_.ptr += static_cast<unsigned long long>(bbIndex_) * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
	dsvHandle_ = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1 , &rtvHandle_ , false , &dsvHandle_);

	//3.��ʃN���A
	//�����_�[�^�[�Q�b�g�N���A
	commandList_->ClearRenderTargetView(rtvHandle_ , clearColor_ , 0 , nullptr);
	
	//�[�x�o�b�t�@�N���A
	commandList_->ClearDepthStencilView(dsvHandle_ , D3D12_CLEAR_FLAG_DEPTH , 1.0f , 0 , 0 , nullptr);

	//4.�`��R�}���h
	//�r���[�|�[�g�ݒ�̃R�}���h
	viewport_.Width = winApp_->WINDOW_WIDTH;			//����
	viewport_.Height = winApp_->WINDOW_HEIGHT - 0;	//�c��

	viewport_.TopLeftX = 0;					//����X
	viewport_.TopLeftY = 0;					//����Y
	viewport_.MinDepth = 0.0f;				//�ŏ��[�x
	viewport_.MaxDepth = 1.0f;				//�ő�[�x

	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetViewports(1 , &viewport_);

	//�V�U�[��`
	scissorRect_.left = 0;											//�؂蔲�����W��
	scissorRect_.right = scissorRect_.left + winApp_->WINDOW_WIDTH;	//�؂蔲�����W�E
	scissorRect_.top = 0;											//�؂蔲�����W��
	scissorRect_.bottom = scissorRect_.top + winApp_->WINDOW_HEIGHT;	//�؂蔲�����W��

	//�V�U�[��`�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetScissorRects(1 , &scissorRect_);

}

void DX12base::PostDraw() {
	//�o�b�N�o�b�t�@�̔ԍ����擾
	bbIndex_ = swapChain_->GetCurrentBackBufferIndex();

	//5.���\�[�X�o���A��߂�
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//�\����Ԃ�
	commandList_->ResourceBarrier(1 , &barrierDesc_);

	//���߂̃N���[�Y
	result = commandList_->Close();
	assert(SUCCEEDED(result));
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists_[] = {commandList_.Get()};
	commandQueue_->ExecuteCommandLists(1 , commandLists_);

	//��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	result = swapChain_->Present(1 , 0);
	assert(SUCCEEDED(result));

	//�R�}���h�̎��s������҂�
	commandQueue_->Signal(fence_.Get() , ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr , false , false , nullptr);
		fence_->SetEventOnCompletion(fenceVal_ , event);
		if (event != 0) {
			WaitForSingleObject(event , INFINITE);
			CloseHandle(event);
		}
	}

	//�L���[���N���A
	result = cmdAllocator_->Reset();
	assert(SUCCEEDED(result));
	//�ĂуR�}���h���X�g�����߂鏀��
	result = commandList_->Reset(cmdAllocator_.Get() , nullptr);

}

void DX12base::CreateCmdList() {

	//�R�}���h�A���P�[�^�𐶐�
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		IID_PPV_ARGS(&cmdAllocator_)
	);
	assert(SUCCEEDED(result));

	//�R�}���h���X�g�𐶐�
	result = device_->CreateCommandList(
		0 ,
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		cmdAllocator_.Get() , nullptr ,
		IID_PPV_ARGS(&commandList_)
	);
	assert(SUCCEEDED(result));

	//�R�}���h�L���[
	//�R�}���h�L���[�𐶐�
	result = device_->CreateCommandQueue(
		&commandQueueDesc_ ,
		IID_PPV_ARGS(&commandQueue_)
	);
	assert(SUCCEEDED(result));

}

void DX12base::SetSwapChain() {

	swapChainDesc_.Width = winApp_->WINDOW_WIDTH;
	swapChainDesc_.Height = winApp_->WINDOW_HEIGHT;
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				//�F���̏���
	swapChainDesc_.SampleDesc.Count = 1;								//�}���`�T���v�����Ȃ�
	swapChainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER;				//�o�b�N�o�b�t�@�p
	swapChainDesc_.BufferCount = 2;									//�o�b�t�@����2�ɐݒ�
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			//�t���b�v��͔j��
	swapChainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//�X���b�v�`�F�[���̐���
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get() ,
		winApp_->GetHwnd() ,
		&swapChainDesc_ ,
		nullptr ,
		nullptr ,
		&swapChain1_
	);
	assert(SUCCEEDED(result));

	//��������IDXGIswapChain1_�̃I�u�W�F�N�g��IDXGIswapChain_4�ɕϊ�����
	swapChain1_.As(&swapChain_);

	//�f�X�N���v�^�q�[�v
	//�f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc_.NumDescriptors = swapChainDesc_.BufferCount;

	//�f�X�N���v�^�q�[�v�̐���
	device_->CreateDescriptorHeap(&rtvHeapDesc_ , IID_PPV_ARGS(&rtvHeap_));

	//�o�b�N�o�b�t�@
	backBuffers_.resize(swapChainDesc_.BufferCount);

	//�����_�[�r���[�^�[�Q�b�g
	//�X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		//�X���b�v�`�F�[������o�b�t�@���擾
		swapChain_->GetBuffer((UINT)i , IID_PPV_ARGS(&backBuffers_[i]));
		//�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		//�����\���ŃA�h���X�������
		rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		//�����_�[�r���[�^�[�Q�b�g�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//�����_�[�r���[�^�[�Q�b�g�̐���
		device_->CreateRenderTargetView(backBuffers_[i].Get() , &rtvDesc , rtvHandle_);

	}

}

void DX12base::DepthTest() {
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApp_->WINDOW_WIDTH;		//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = winApp_->WINDOW_WIDTH;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	depthHeapProp_.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	depthClearValue_.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue_.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//���\�[�X����
	result = device_->CreateCommittedResource(
		&depthHeapProp_ ,
		D3D12_HEAP_FLAG_NONE ,
		&depthResourceDesc ,
		D3D12_RESOURCE_STATE_DEPTH_WRITE ,	//�[�x�l�������݂Ɏg�p
		&depthClearValue_ ,
		IID_PPV_ARGS(&depthBuff_)
	);

	//�[�x�r���[�p�f�X�N���v�^�q�[�v
	dsvHeapDesc_.NumDescriptors = 1;	//�[�x�r���[�͈��
	dsvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[
	result = device_->CreateDescriptorHeap(&dsvHeapDesc_ , IID_PPV_ARGS(&dsvHeap_));

	//�[�x�r���[�쐬
	dsvDesc_.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get() ,
		&dsvDesc_ ,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);
}

void DX12base::CreateFence(){

	//�t�F���X�̐���
	result = device_->CreateFence(fenceVal_ , D3D12_FENCE_FLAG_NONE , IID_PPV_ARGS(&fence_));

	assert(SUCCEEDED(result));

}

//�A�N�Z�b�T
ComPtr<ID3D12GraphicsCommandList> DX12base::GetCmdList() {
	return commandList_.Get();
}

ComPtr<ID3D12Device> DX12base::GetDevice() {
	return device_.Get();
}