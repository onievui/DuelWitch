#include "Sources\Game\MyGame.h"

// �E�B���h�E��
constexpr int width = 1600;
// �E�B���h�E��
constexpr int height = 900;

// �G���g���|�C���g
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	hInstance, nCmdShow;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// DirectXMath���C�u���������݂̃v���b�g�t�H�[�����T�|�[�g���Ă��邩�m�F����
	if (!DirectX::XMVerifyCPUSupport()) {
		return 1;
	}

	// COM���C�u����������������
	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) {
		return 1;
	}

	// MyGame�I�u�W�F�N�g�𐶐�����
	MyGame myGame(width, height);
	// �Q�[�������s����
	MSG msg = myGame.Run();

	// Com���C�u�������I����������
	CoUninitialize();
	return static_cast<int>(msg.wParam);
}
