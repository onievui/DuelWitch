#pragma once
#ifndef  WINDOW_DEFINED
#define  WINDOW_DEFINED


#include "Game.h"


constexpr wchar_t* CLASS_NAME = L"Duel Witch";
constexpr wchar_t* WINDOW_NAME = L"Duel Witch";


/// <summary>
/// �E�C���h�E�N���X
/// </summary>
class Window {
public:
	// �R���X�g���N�^
	Window(HINSTANCE hInstance, int nCmdShow) : m_hInstance(hInstance), m_nCmdShow(nCmdShow) {}

public:
	// �E�B���h�E�n���h�����擾����
	HWND GetHWnd() const { return m_hWnd; }

	// �N���X��o�^���E�B���h�E�𐶐�����
	int Initialize(int width, int height);

	// �E�B���h�E��\������
	void ShowWindow();

	// �E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WndowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	// �E�B���h�E�n���h��
	HWND      m_hWnd;
	// �C���X�^���X�n���h��
	HINSTANCE m_hInstance;
	// �E�B���h�E�\���t���O
	int       m_nCmdShow;
};


#endif	// WINDOW_DEFINED
