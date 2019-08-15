#include "LogoScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"


/// <summary>
/// コンストラクタ
/// </summary>
LogoScene::LogoScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
LogoScene::~LogoScene() {
}

/// <summary>
///	ロゴシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void LogoScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());
	// スプライトフォントを生成する
	//m_font = std::make_unique<DirectX::SpriteFont>(DirectX11::Get().GetDevice().Get(), L"myfile.spritefont");

	// キートラッカーを生成する
	m_tracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

}

/// <summary>
/// ロゴシーンを更新する
/// </summary>
/// <param name="timer"></param>
void LogoScene::Update(const DX::StepTimer& timer) {
	timer;
	m_tracker->Update(DirectX::Keyboard::Get().GetState());

	if (m_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
		m_pSceneRequest->RequestScene("Play");
	}
}

/// <summary>
/// ロゴシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void LogoScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin();

	spriteBatch->End();
}

/// <summary>
/// ロゴシーンを終了する
/// </summary>
void LogoScene::Finalize() {

}

