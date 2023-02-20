#include <cassert>

#include <math.h>
#include <random>

#include "WinApp.h"
#include "Input.h"
#include "DX12base.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "WorldTransform.h"


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
	DX12base* dx12base = new DX12base;
	dx12base->Initialize(winApp);

	//スプライト共通処理
	SpriteCommon* spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dx12base);

	// キーボードデバイスの生成
	Input* input = new Input();
	input->Initialize(winApp);

	//ゲームループ前初期化処理
	spriteCommon->LoadTexture(0 , "texture.jpg");
	spriteCommon->LoadTexture(1 , "reimu.png");

	//スプライト
	Sprite* sprite = new Sprite;
	sprite->Initialize(spriteCommon , 1);


	//ゲームループ
	while (true) {

		//ウィンドウメッセージ処理
		if (winApp->ProcMessage()) {
			break;
		}

		//DirectX毎フレーム処理
		//更新処理
		input->Update();
		if (input->TriggerKey(DIK_SPACE)) {
			if (sprite->GetTextureIndex() == 0) {
				sprite->SetTextureIndex(1);
			}
			else {
				sprite->SetTextureIndex(0);
			}
		}

		if (input->PushKey(DIK_RIGHT)) {
			sprite->SetPosition(Vector2{sprite->GetPosition().x + 1 , sprite->GetPosition().y});
		}
		if (input->PushKey(DIK_LEFT)) {
			sprite->SetPosition(Vector2{sprite->GetPosition().x - 1 , sprite->GetPosition().y});
		}
		if (input->PushKey(DIK_UP)) {
			sprite->SetPosition(Vector2{sprite->GetPosition().x , sprite->GetPosition().y - 1});
		}
		if (input->PushKey(DIK_DOWN)) {
			sprite->SetPosition(Vector2{sprite->GetPosition().x , sprite->GetPosition().y + 1});
		}

		if (input->PushKey(DIK_Q)) {
			sprite->SetRotation(sprite->GetRotation() - MathFunc::Utility::Deg2Rad(1));
		}
		if (input->PushKey(DIK_W)) {
			sprite->SetRotation(sprite->GetRotation() + MathFunc::Utility::Deg2Rad(1));
		}

		if (input->PushKey(DIK_A)) {
			sprite->SetScale(Vector2{sprite->GetScale().x - 0.1f , sprite->GetScale().y});
		}
		if (input->PushKey(DIK_S)) {
			sprite->SetScale(Vector2{sprite->GetScale().x + 0.1f , sprite->GetScale().y});
		}
		if (input->PushKey(DIK_Z)) {
			sprite->SetScale(Vector2{sprite->GetScale().x , sprite->GetScale().y - 0.1f});
		}
		if (input->PushKey(DIK_X)) {
			sprite->SetScale(Vector2{sprite->GetScale().x , sprite->GetScale().y + 0.1f});
		}

		if (input->TriggerKey(DIK_R)) {
			if (sprite->GetIsFlipX() == false) {
				sprite->SetIsFlipX(true);
			}
			else {
				sprite->SetIsFlipX(false);
			}
		}

		if (input->TriggerKey(DIK_F)) {
			if (sprite->GetIsFlipY() == false) {
				sprite->SetIsFlipY(true);
			}
			else {
				sprite->SetIsFlipY(false);
			}
		}

		if (input->TriggerKey(DIK_V)) {
			if (sprite->GetIsVisible() == false) {
				sprite->SetIsVisible(true);
			}
			else {
				sprite->SetIsVisible(false);
			}
		}

		sprite->Update();

		//描画処理
		dx12base->PreDraw();

		//スプライト描画
		spriteCommon->PreDraw();
		sprite->Draw();

		dx12base->PostDraw();

	}

	//ID3D12DebugDevice* debugInterface;

	//if (SUCCEEDED(dx12base->GetDevice().Get()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}

	//終了時処理
	delete input;

	delete sprite;
	delete spriteCommon;

	delete dx12base;

	winApp->Finalize();
	delete winApp;

	return 0;
}