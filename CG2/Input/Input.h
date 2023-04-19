#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInoutのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>

#include "WinApp.h"

class Input {

	//メンバ関数
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//初期化
	void Initialize(WinApp* winApp);

	//更新
	void Update();

	//キーの入力情報を取得する関数
	//プッシュ
	bool PushKey(BYTE keyName);

	//トリガー
	bool TriggerKey(BYTE keyName);

	//リリース
	bool ReleaseKey(BYTE keyName);

	//メンバ変数
private:
	//DirectInput
	ComPtr<IDirectInput8> directInput_ = nullptr;

	//キーボード
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	BYTE key_[256] = {};
	BYTE oldKey_[256] = {};

	WinApp* winApp_ = nullptr;
};

