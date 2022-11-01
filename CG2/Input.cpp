#include "Input.h"

void Input::Initialize(WNDCLASSEX w,HWND hwnd) {

	HRESULT result;

	//Directinput�̏�����
	result = DirectInput8Create(
		w.hInstance ,
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
		hwnd , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
int Input::TriggerKey(int keyName) {

	if (key_[keyName] == true&&oldKey_[keyName]==false) {
		return true;
	}
	else {
		return false;
	}

}

int Input::PushKey(int keyName) {

	if (key_[keyName] == true) {
		return true;
	}
	else {
		return false;
	}

}

int Input::ReleaseKey(int keyName) {

	if (key_[keyName] == false && oldKey_[keyName] == true) {
		return true;
	}
	else {
		return false;
	}

}