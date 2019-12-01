#include "Window.h"


/// <summary>
/// �N���X��o�^���ăE�C���h�E�n���h���𐶐�����
/// </summary>
/// <param name="width">�o�͕�</param>
/// <param name="height">�o�͍�</param>
/// <returns>
/// 0 : ����
/// 1 : ���s
/// </returns>
int Window::Initialize(int width, int height) {
	// �N���X��o�^����
	WNDCLASSEX wnd_class_ex;
	wnd_class_ex.cbSize = sizeof(WNDCLASSEX);
	wnd_class_ex.style = CS_HREDRAW | CS_VREDRAW;
	wnd_class_ex.lpfnWndProc = WndowProc;
	wnd_class_ex.cbClsExtra = 0;
	wnd_class_ex.cbWndExtra = 0;
	wnd_class_ex.hInstance = m_hInstance;
	wnd_class_ex.hIcon = LoadIcon(m_hInstance, L"IDI_ICON");
	wnd_class_ex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd_class_ex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd_class_ex.lpszMenuName = nullptr;
	wnd_class_ex.lpszClassName = CLASS_NAME;
	wnd_class_ex.hIconSm = LoadIcon(wnd_class_ex.hInstance, L"IDI_ICON");
	// �N���X��o�^����
	if (!RegisterClassEx(&wnd_class_ex)) {
		return EXIT_FAILURE;
	}
	// �E�B���h�E�𐶐�����
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = static_cast<LONG>(width);
	rect.bottom = static_cast<LONG>(height);

	// �E�C���h�E�X�^�C���̐ݒ������
	DWORD dw_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// �N���C�A���g�̈�̃T�C�Y�ɂ��E�B���h�E�T�C�Y��ύX����
	AdjustWindowRect(&rect, dw_style, FALSE);

	// �E�B���h�E�𐶐�����
	m_hWnd = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, dw_style,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd) {
		return EXIT_FAILURE;
	}
	// TODO: nCmdShow��SW_SHOWMAXIMIZED���Z�b�g���ċK��Ńt���X�N���[���ɕύX����
	// SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));

	// GetClientRect(hwnd, &rc);
	// Initialize window
	// g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
	return EXIT_SUCCESS;
}

/// <summary>
/// �E�C���h�E��\������
/// </summary>
void Window::ShowWindow() {
	// �E�B���h�E��\������
	::ShowWindow(m_hWnd, m_nCmdShow);
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK Window::WndowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;

	// TODO: �K��Ńt���X�N���[���ɂ���ꍇs_fullscreen��true�ɐݒ肷��
	Game* game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) {
			if (!s_minimized) {
				s_minimized = true;
				if (!s_in_suspend && game) {
					game->OnSuspending();
				}
				s_in_suspend = true;
			}
		}
		else if (s_minimized) {
			s_minimized = false;
			if (s_in_suspend && game) {
				game->OnResuming();
			}
			s_in_suspend = false;
		}
		else if (!s_in_sizemove && game) {
			game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (game) {
			RECT rc;
			GetClientRect(hWnd, &rc);
			game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;

	case WM_GETMINMAXINFO: {
			MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 320;
			info->ptMinTrackSize.y = 200;
		}
		break;

	case WM_ACTIVATEAPP:
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		if (game) {
			if (wParam) {
				game->OnActivated();
			}
			else {
				game->OnDeactivated();
			}
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam) {
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend && game) {
				game->OnSuspending();
			}
			s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized) {
				if (s_in_suspend && game) {
					game->OnResuming();
				}
				s_in_suspend = false;
			}
			return TRUE;
		}
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000) {
			// ALT+ENTER �t���X�N���[�� �g�O������������
			if (s_fullscreen) {
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

				int width = 800;
				int height = 600;
				if (game) {
					game->GetDefaultSize(width, height);
				}
				::ShowWindow(hWnd, SW_SHOWNORMAL);
				::SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else {
				SetWindowLongPtr(hWnd, GWL_STYLE, 0);
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
				::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
				::ShowWindow(hWnd, SW_SHOWMAXIMIZED);
			}

			s_fullscreen = !s_fullscreen;
		}
		break;

	case WM_MENUCHAR:
		// ���j���[���A�N�e�B�u�Ń��[�U�[���j�[���j�b�N�܂��̓A�N�Z�����[�^�L�[�Ɉ�v���Ȃ��L�[��
		// �������ꍇ�ł������𖳎����G���[�r�[�v��炳�Ȃ�
		return MAKELRESULT(0, MNC_CLOSE);

	// �}�E�X�֘A���b�Z�[�W
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;

		// �L�[�{�[�h�֘A���b�Z�[�W
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
