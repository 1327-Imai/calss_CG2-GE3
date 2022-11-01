#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInoutのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>


class Input {

	//メンバ関数
public:

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//初期化
	void Initialize(WNDCLASSEX w,HWND hwnd);

	//更新
	void Update();

	//キーの入力情報を取得する関数
	//プッシュ
	int PushKey(int keyName);

	//トリガー
	int TriggerKey(int keyName);

	//リリース
	int ReleaseKey(int keyName);

	//メンバ変数
private:
	//DirectInput
	ComPtr<IDirectInput8> directInput_ = nullptr;

	//キーボード
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	BYTE key_[256] = {};
	BYTE oldKey_[256] = {};

};

