#pragma once
#ifndef  WINDOW_DEFINED
#define  WINDOW_DEFINED


#include "Game.h"


constexpr wchar_t* CLASS_NAME = L"Duel Witch";
constexpr wchar_t* WINDOW_NAME = L"Duel Witch";


/// <summary>
/// ウインドウクラス
/// </summary>
class Window {
public:
	// コンストラクタ
	Window(HINSTANCE hInstance, int nCmdShow) : m_hInstance(hInstance), m_nCmdShow(nCmdShow) {}

public:
	// ウィンドウハンドルを取得する
	HWND GetHWnd() const { return m_hWnd; }

	// クラスを登録しウィンドウを生成する
	int Initialize(int width, int height);

	// ウィンドウを表示する
	void ShowWindow();

	// ウィンドウプロシージャ
	static LRESULT CALLBACK WndowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	// ウィンドウハンドル
	HWND      m_hWnd;
	// インスタンスハンドル
	HINSTANCE m_hInstance;
	// ウィンドウ表示フラグ
	int       m_nCmdShow;
};


#endif	// WINDOW_DEFINED
