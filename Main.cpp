#include "Sources\Game\MyGame.h"

// ウィンドウ幅
constexpr int width = 1600;
// ウィンドウ高
constexpr int height = 900;

// エントリポイント
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	hInstance, nCmdShow;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// DirectXMathライブラリが現在のプラットフォームをサポートしているか確認する
	if (!DirectX::XMVerifyCPUSupport()) {
		return 1;
	}

	// COMライブラリを初期化する
	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) {
		return 1;
	}

	// MyGameオブジェクトを生成する
	MyGame myGame(width, height);
	// ゲームを実行する
	MSG msg = myGame.Run();

	// Comライブラリを終了処理する
	CoUninitialize();
	return static_cast<int>(msg.wParam);
}
