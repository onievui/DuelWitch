#define _CRT_SECURE_NO_WARNINGS


#include "MyGame.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Game\Load\CommonServices.h>
#include <Game\Load\ResourceLoader.h>
#include <Game\Scene\SceneManager.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="width">ウインドウ幅</param>
/// <param name="height">ウインドウ高</param>
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

	// 背景色を黒にする
	Game::SetBackGroundColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));

	// キーボードを生成する
	m_keyboard = std::make_unique<DirectX::Keyboard>();

	// コモンサービスを初期化する
	m_commonServices = std::make_unique<CommonServices>();
	m_commonServices->Initialize();

	// マウスの感度を設定する
	// TODO: マウス感度を外部から読み込む
	ServiceLocater<MouseWrapper>::Get()->SetSensivity(4.0f);

	// フォントを読み込む
	ResourceLoader::Load(ResourceLoaderID::Common);

	// シーンマネージャを初期化する
	m_sceneManager = std::make_unique<SceneManager>();
	m_sceneManager->Initialize();

	// プレイシーンを呼び出す
	m_sceneManager->RequestScene(SceneID::Logo);
}

/// <summary>
/// リソースを生成する
/// </summary>
void MyGame::CreateResources() {
	// 基底クラスのCreateResourcesを呼び出す
	Game::CreateResources();

}

// ゲームを更新する
void MyGame::Update(const DX::StepTimer& timer) {
	// キートラッカーを更新する
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->Update(m_keyboard->GetState());
	// マウストラッカーを更新する
	ServiceLocater<MouseWrapper>::Get()->Update(m_width, m_height);
	// オーディオマネージャを更新する
	ServiceLocater<AudioManager>::Get()->Update(timer);

	// シーンを更新する
	m_sceneManager->Update(timer);
	
}

// ゲームを描画する
void MyGame::Render(const DX::StepTimer& timer) {
	// 最初の更新の前は何も描画しないようにする
	if (timer.GetFrameCount() == 0) {
		return;
	}

	// バッファをクリアする
	Clear();

	// シーンを描画する
	m_sceneManager->Render(GetSpriteBatch());

	// バックバッファを表示する
	Present();
}

// 後始末をする
void MyGame::Finalize() {
	// 基底クラスのFinalizeを呼び出す
	Game::Finalize();

	ResourceLoader::Release(ResourceLoaderID::Common);
	m_commonServices->Finalize();

	m_keyboard.reset();
}

// FPSを描画する
void MyGame::DrawFPS(const DX::StepTimer& timer) {
	// FPS文字列を生成する
	std::wstring fpsString = L"fps = " + std::to_wstring(static_cast<unsigned int>(timer.GetFramesPerSecond()));
	// FPSを描画する
	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(GetSpriteBatch(), fpsString.c_str(), DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);
}
