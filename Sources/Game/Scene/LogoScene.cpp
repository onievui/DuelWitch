#include "LogoScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\Fade.h>


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
/// <param name="pSceneRequest">リクエストシーンインタフェース</param>
void LogoScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	// リソースをロードする
	ResourceLoader::Load(ResourceLoaderID::LogoScene);
	// フェードを生成する
	m_fade = std::make_unique<Fade>();
	m_fade->Start();
	m_fadeStep = 0;
}

/// <summary>
/// ロゴシーンを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void LogoScene::Update(const DX::StepTimer& timer) {
	// エスケープキーで終了
	const bool press_escape = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape);
	if (press_escape) {
		ExitGame();
	}

	// スペースキーを押すか左クリック、または時間経過でタイトルシーンに移行する
	const bool press_space = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space);
	const bool click_left = (ServiceLocater<MouseWrapper>::Get()->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED);
	const bool time_end = (m_fadeStep > 4 && m_fade->IsFinished());
	if (press_space || click_left || time_end) {
		m_pSceneRequest->RequestScene(SceneID::Title);
	}

	// フェードを更新する
	m_fade->Update(timer);
	// フェードの段階を更新する
	if (m_fade->IsFinished() && m_fade->IsRunning()) {
		switch (m_fadeStep) {
		// 少し待つ
		case 0:
			m_fade->Initialize(Fade::State::NoFade, 1.0f, 1.0f);
			break;
		// フェードインする
		case 1:
			m_fade->Initialize(Fade::State::FadeIn, 1.0f, 0.0f);
			break;
		// フェードをいったん止める
		case 2:
			m_fade->Initialize(Fade::State::NoFade, 1.0f, 0.0f);
			break;
		// フェードアウトする
		case 3:
			m_fade->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
			break;
		// フェードを止める
		case 4:
			m_fade->Initialize(Fade::State::NoFade, 1.0f, 1.0f);
			break;
		default:
			m_fade->Stop();
			break;
		}
		++m_fadeStep;
	}
	
}

/// <summary>
/// ロゴシーンを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void LogoScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Logo);
	
	// ロゴを描画する
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::Colors::White, 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// 時間経過でフェードイン・フェードアウトをする
	m_fade->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// ロゴシーンを終了する
/// </summary>
void LogoScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::LogoScene);
}

