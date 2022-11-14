#include "Input.h"

void Input::Initialize(WinApp* winApp) {

	HRESULT result;
	
	//nullptrチェック
	assert(winApp);

	winApp_ = winApp;

	//Directinputの初期化
	result = DirectInput8Create(
		winApp->GetHInstance() ,
		DIRECTINPUT_VERSION ,
		IID_IDirectInput8 ,
		(void**)&directInput_ ,
		nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard , &keyboard_ , NULL);
	assert(SUCCEEDED(result));

	//入力データのリセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard); //標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのリセット
	result = keyboard_->SetCooperativeLevel(
		winApp->GetHwnd() , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

}

void Input::Update() {

	memcpy(oldKey_ , key_ , sizeof(key_));

	//キーボード情報の取得開始
	keyboard_->Acquire();

	//前キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(key_) , key_);

}

//キーの入力情報を取得する関数
bool Input::TriggerKey(BYTE keyName) {

	if (key_[keyName] && !oldKey_[keyName]) {
		return true;
	}
	else {
		return false;
	}

}

bool Input::PushKey(BYTE keyName) {

	if (key_[keyName]) {
		return true;
	}
	else {
		return false;
	}

}

bool Input::ReleaseKey(BYTE keyName) {

	if (!key_[keyName] && oldKey_[keyName]) {
		return true;
	}
	else {
		return false;
	}

}