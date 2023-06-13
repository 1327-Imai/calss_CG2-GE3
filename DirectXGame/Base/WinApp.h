#pragma once
#include <windows.h>

class WinApp {

	//メンバ関数
public:
	//初期化
	void Initialize();

	//終了時処理
	void Finalize();

	bool ProcMessage();

private:
	//ウィンドウプロシーシャ
	static LRESULT WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam);

	//アクセッサ
public:
	HWND GetHwnd();

	HINSTANCE GetHInstance();

	//メンバ変数
public:
	//ウィンドウサイズ
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

private:
	//ウィンドウクラス
	WNDCLASSEX w_{};

	//ウィンドウハンドル
	HWND hwnd_;

};

