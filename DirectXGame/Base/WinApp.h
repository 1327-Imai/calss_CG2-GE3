#pragma once
#include <windows.h>

class WinApp {

	//�����o�֐�
public:
	//������
	void Initialize();

	//�I��������
	void Finalize();

	bool ProcMessage();

private:
	//�E�B���h�E�v���V�[�V��
	static LRESULT WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam);

	//�A�N�Z�b�T
public:
	HWND GetHwnd();

	HINSTANCE GetHInstance();

	//�����o�ϐ�
public:
	//�E�B���h�E�T�C�Y
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

private:
	//�E�B���h�E�N���X
	WNDCLASSEX w_{};

	//�E�B���h�E�n���h��
	HWND hwnd_;

};

