#define _CRT_SECURE_NO_WARNINGS

#include "MyGame.h"
#include "SceneManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="width">画面横幅</param>
/// <param name="height">画面縦幅</param>
MyGame::MyGame(int width, int height) : m_width(width), m_height(height), Game(width, height) {
}

/// <summary>
/// デストラクタ
/// </summary>
MyGame::~MyGame() {
}

/// <summary>
/// ゲームを初期化する
/// </summary>
/// <param name="width">画面横幅</param>
/// <param name="height">画面縦幅</param>
void MyGame::Initialize(int width, int height) {
	// 基底クラスのInitializeを呼び出す 
	Game::Initialize(width, height);

	// キーボードを生成する
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	// マウスを生成する
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(DirectX11::Get().GetHWnd());
	// シーンマネージャを初期化する
	m_sceneManager = std::make_unique<SceneManager>();
	m_sceneManager->Initialize();
	// プレイシーンを呼び出す
	m_sceneManager->RequestScene("Play");

}

/// <summary>
/// リソースを生成する
/// </summary>
void MyGame::CreateResources() {
	// 基底クラスのCreateResourcesを呼び出す
	Game::CreateResources();

}

// ゲームを更新する
void MyGame::Update(const DX::StepTimer& timer) 
{
	// シーンを更新する
	m_sceneManager->Update(timer);
	
}

// ゲームを描画する
void MyGame::Render(const DX::StepTimer& timer) 
{
	// 最初の更新の前は何も描画しないようにする
	if (timer.GetFrameCount() == 0) 
		return;

	// TODO: レンダリングコードを追加する
	float time = float(timer.GetTotalSeconds());

	// バッファをクリアする
	Clear();

	// シーンを描画する
	m_sceneManager->Render(GetSpriteBatch());

	// バックバッファを表示する
	Present();
}

// 後始末をする
void MyGame::Finalize() 
{
	// 基底クラスのFinalizeを呼び出す
	Game::Finalize();

	m_keyboard.reset();
	m_mouse.reset();
}

// FPSを描画する
void MyGame::DrawFPS(const DX::StepTimer& timer)
{
	// FPS文字列を生成する
	std::wstring fpsString = L"fps = " + std::to_wstring((unsigned int)timer.GetFramesPerSecond());
	// FPSを描画する
	GetSpriteFont()->DrawString(GetSpriteBatch(), fpsString.c_str(), DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);
}
