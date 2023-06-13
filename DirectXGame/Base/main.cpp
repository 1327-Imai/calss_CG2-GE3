#include <cassert>

#include <math.h>
#include <random>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "GameScene.h"

using namespace DirectX;
using namespace Microsoft::WRL;

const double PI = 3.141592;

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
std::uniform_real_distribution<float> distPosX(-100.0 , 100.0);
std::uniform_real_distribution<float> distPosY(-50.0 , 50.0);
std::uniform_real_distribution<float> distPosZ(30.0 , 60.0);
std::uniform_real_distribution<float> distRot(0 , XMConvertToRadians(360.0f));

//main関数
int WINAPI WinMain(_In_ HINSTANCE , _In_opt_ HINSTANCE , _In_ LPSTR , _In_ int) {
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!/n");

	//ウィンドウの生成
	//WindowsAPI
	WinApp* winApp = new WinApp;
	winApp->Initialize();

	//メッセージループ
	MSG msg{}; //メッセージ

	//デバッグレイヤーの有効化
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif

	//DirectX基礎
	DirectXCommon* dxCommon = new DirectXCommon;
	dxCommon->Initialize(winApp);

	GameScene* gameScene = new GameScene;
	gameScene->Initialize(winApp,dxCommon);

	//ゲームループ
	while (true) {

		//ウィンドウメッセージ処理
		if (winApp->ProcMessage()) {
			break;
		}

		//DirectX毎フレーム処理
		//更新処理
		gameScene->Update();

		//描画処理
		dxCommon->PreDraw();

		gameScene->Draw();

		dxCommon->PostDraw();

	}

	//ID3D12DebugDevice* debugInterface;

	//if (SUCCEEDED(dxCommon->GetDevice().Get()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}

	//終了時処理
	delete gameScene;

	delete dxCommon;

	winApp->Finalize();
	delete winApp;

	return 0;
}