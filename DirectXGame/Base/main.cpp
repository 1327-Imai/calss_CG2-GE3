#include <cassert>

#include <math.h>
#include <random>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "GameScene.h"

using namespace DirectX;
using namespace Microsoft::WRL;

const double PI = 3.141592;

//�����V�[�h������
std::random_device seed_gen;
//�����Z���k�E�c�C�X�^�[�̗����G���W��
std::mt19937_64 engine(seed_gen());
//�����͈͂̎w��
std::uniform_real_distribution<float> distPosX(-100.0 , 100.0);
std::uniform_real_distribution<float> distPosY(-50.0 , 50.0);
std::uniform_real_distribution<float> distPosZ(30.0 , 60.0);
std::uniform_real_distribution<float> distRot(0 , XMConvertToRadians(360.0f));

//main�֐�
int WINAPI WinMain(_In_ HINSTANCE , _In_opt_ HINSTANCE , _In_ LPSTR , _In_ int) {
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!/n");

	//�E�B���h�E�̐���
	//WindowsAPI
	WinApp* winApp = new WinApp;
	winApp->Initialize();

	//���b�Z�[�W���[�v
	MSG msg{}; //���b�Z�[�W

	//�f�o�b�O���C���[�̗L����
#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif

	//DirectX��b
	DirectXCommon* dxCommon = new DirectXCommon;
	dxCommon->Initialize(winApp);

	GameScene* gameScene = new GameScene;
	gameScene->Initialize(winApp,dxCommon);

	//�Q�[�����[�v
	while (true) {

		//�E�B���h�E���b�Z�[�W����
		if (winApp->ProcMessage()) {
			break;
		}

		//DirectX���t���[������
		//�X�V����
		gameScene->Update();

		//�`�揈��
		dxCommon->PreDraw();

		gameScene->Draw();

		dxCommon->PostDraw();

	}

	//ID3D12DebugDevice* debugInterface;

	//if (SUCCEEDED(dxCommon->GetDevice().Get()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}

	//�I��������
	delete gameScene;

	delete dxCommon;

	winApp->Finalize();
	delete winApp;

	return 0;
}