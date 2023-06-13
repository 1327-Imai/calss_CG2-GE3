#include "WinApp.h"

#pragma comment(lib,"winmm.lib")

void WinApp::Initialize() {

#pragma region//�E�B���h�E�̐���

	//�E�B���h�E�N���X�̐ݒ�
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc;		//�E�B���h�E�v���V�[�V����ݒ�
	w_.lpszClassName = L"DirectXGame";			//�E�B���h�E�N���X��
	w_.hInstance = GetModuleHandle(nullptr);		//�E�B���h�E�n���h��
	w_.hCursor = LoadCursor(NULL , IDC_ARROW);	//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w_);
	//�E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = {0 , 0 , WINDOW_WIDTH , WINDOW_HEIGHT};
	//�����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc , WS_OVERLAPPEDWINDOW , false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd_ = CreateWindow(
		w_.lpszClassName ,		//�N���X��
		L"DirectXGame" ,		//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW ,	//�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT ,			//�\��X���W(OS�ɔC����)
		CW_USEDEFAULT ,			//�\��Y���W(OS�ɔC����)
		wrc.right - wrc.left ,	//�E�B���h�E����
		wrc.bottom - wrc.top ,	//�E�B���h�E�c��
		nullptr ,				//�e�E�B���h�E�n���h��
		nullptr ,				//���j���[�n���h��
		w_.hInstance ,			//�Ăяo���A�v���P�[�V�����n���h��
		nullptr					//�I�v�V����
	);

	//�E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd_ , SW_SHOW);

	//�V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);

#pragma endregion//�E�B���h�E�̐���

}

void WinApp::Finalize() {
	//�E�B���h�E�N���X�o�^����
	UnregisterClass(w_.lpszClassName , w_.hInstance);
}

bool WinApp::ProcMessage() {

	MSG msg{};

	//���b�Z�[�W������?
	if (PeekMessage(&msg , nullptr , 0 , 0 , PM_REMOVE)) {
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�V���Ƀ��b�Z�[�W�𑗂�
	}

	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

//�E�B���h�E�v���V�[�V��
LRESULT WinApp::WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam) {

	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
	//OS�ɑ΂��āA�A�v���̏I����`����
	PostQuitMessage(0);
	return 1;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd , msg , wparam , lparam);
}

HWND WinApp::GetHwnd() {
	return hwnd_;
}

HINSTANCE WinApp::GetHInstance() {
	return w_.hInstance;
}