#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInout�̃o�[�W�����w��
#include <dinput.h>
#include <cassert>
#include <wrl.h>

#include "WinApp.h"

class Input {

	//�����o�֐�
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//������
	void Initialize(WinApp* winApp);

	//�X�V
	void Update();

	//�L�[�̓��͏����擾����֐�
	//�v�b�V��
	bool PushKey(BYTE keyName);

	//�g���K�[
	bool TriggerKey(BYTE keyName);

	//�����[�X
	bool ReleaseKey(BYTE keyName);

	//�����o�ϐ�
private:
	//DirectInput
	ComPtr<IDirectInput8> directInput_ = nullptr;

	//�L�[�{�[�h
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	BYTE key_[256] = {};
	BYTE oldKey_[256] = {};

	WinApp* winApp_ = nullptr;
};

