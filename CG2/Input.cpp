#include "Input.h"

void Input::Initialize(WinApp* winApp) {

	HRESULT result;
	
	//nullptr�`�F�b�N
	assert(winApp);

	winApp_ = winApp;

	//Directinput�̏�����
	result = DirectInput8Create(
		winApp->GetHInstance() ,
		DIRECTINPUT_VERSION ,
		IID_IDirectInput8 ,
		(void**)&directInput_ ,
		nullptr);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput_->CreateDevice(GUID_SysKeyboard , &keyboard_ , NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�̃��Z�b�g
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard); //�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃��Z�b�g
	result = keyboard_->SetCooperativeLevel(
		winApp->GetHwnd() , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

}

void Input::Update() {

	memcpy(oldKey_ , key_ , sizeof(key_));

	//�L�[�{�[�h���̎擾�J�n
	keyboard_->Acquire();

	//�O�L�[�̓��͏�Ԃ��擾����
	keyboard_->GetDeviceState(sizeof(key_) , key_);

}

//�L�[�̓��͏����擾����֐�
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