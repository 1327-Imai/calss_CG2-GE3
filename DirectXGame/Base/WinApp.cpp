#include "WinApp.h"

#pragma comment(lib,"winmm.lib")

void WinApp::Initialize() {

#pragma region//ウィンドウの生成

	//ウィンドウクラスの設定
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc;		//ウィンドウプロシーシャを設定
	w_.lpszClassName = L"DirectXGame";			//ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr);		//ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL , IDC_ARROW);	//カーソル指定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w_);
	//ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = {0 , 0 , WINDOW_WIDTH , WINDOW_HEIGHT};
	//自動でサイズを補正する
	AdjustWindowRect(&wrc , WS_OVERLAPPEDWINDOW , false);

	//ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(
		w_.lpszClassName ,		//クラス名
		L"DirectXGame" ,		//タイトルバーの文字
		WS_OVERLAPPEDWINDOW ,	//標準的なウィンドウスタイル
		CW_USEDEFAULT ,			//表示X座標(OSに任せる)
		CW_USEDEFAULT ,			//表示Y座標(OSに任せる)
		wrc.right - wrc.left ,	//ウィンドウ横幅
		wrc.bottom - wrc.top ,	//ウィンドウ縦幅
		nullptr ,				//親ウィンドウハンドル
		nullptr ,				//メニューハンドル
		w_.hInstance ,			//呼び出しアプリケーションハンドル
		nullptr					//オプション
	);

	//ウィンドウを表示状態にする
	ShowWindow(hwnd_ , SW_SHOW);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

#pragma endregion//ウィンドウの生成

}

void WinApp::Finalize() {
	//ウィンドウクラス登録解除
	UnregisterClass(w_.lpszClassName , w_.hInstance);
}

bool WinApp::ProcMessage() {

	MSG msg{};

	//メッセージがある?
	if (PeekMessage(&msg , nullptr , 0 , 0 , PM_REMOVE)) {
		TranslateMessage(&msg);	//キー入力メッセージの処理
		DispatchMessage(&msg);	//プロシーシャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

//ウィンドウプロシーシャ
LRESULT WinApp::WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam) {

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
	//OSに対して、アプリの終了を伝える
	PostQuitMessage(0);
	return 1;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd , msg , wparam , lparam);
}

HWND WinApp::GetHwnd() {
	return hwnd_;
}

HINSTANCE WinApp::GetHInstance() {
	return w_.hInstance;
}