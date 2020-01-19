#include "TitleScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\AudioManager.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\SoundScaleUpUI.h>
#include <Game\UI\Fade.h>


/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene() {
}

/// <summary>
///	タイトルシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void TitleScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	// リソースをロードする
	ResourceLoader::Load(ResourceLoaderID::TitleScene);
	m_wasSelected = false;

	// UIを初期化する
	InitializeUI();

	// フェードを生成する
	m_fadeScreen = std::make_unique<Fade>();
	m_fadeScreen->Start();
	m_fadeScreenStep = 0;

	m_fadeUI = std::make_unique<Fade>();
	// 後から動作を開始させる
	m_fadeUI->Stop();
	m_fadeUIStep = 0;

	// BGMを再生する
	ServiceLocater<AudioManager>::Get()->PlayBgm(BgmID::Title);
	// BGMをフェードインさせる
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Title, 0, 0.9f, 0.0f, 1.0f);
}

/// <summary>
/// タイトルシーンを更新する
/// </summary>
/// <param name="timer"></param>
void TitleScene::Update(const DX::StepTimer& timer) {
	// エスケープキーで終了
	const bool press_escape = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape);
	if (press_escape) {
		ExitGame();
	}

	// スペースキーを押すか左クリックした場合
	const bool press_space = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space);
	const bool click_left = (ServiceLocater<MouseWrapper>::Get()->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED);
	if (press_space || click_left) {
		// タイトルが表示されていなければ、タイマーを進めて表示させる
		if (m_fadeScreenStep < 3) {
			m_fadeScreenStep = 2;
			m_fadeScreen->SkipTime();
		}
	}

	// UIのアルファ値を更新する
	for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetAlpha(m_fadeUI->GetAlpha());
	}

	// 未選択でフェードが完了していたらUIを更新する
	const bool ui_fade_end = (m_fadeUIStep == 2);
	if (!m_wasSelected && ui_fade_end) {
		for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}

		// イベントを取得しているかどうか確認する
		if (m_uiObserver->HasNewEvent()) {
			UIEventID event_id = m_uiObserver->GetEventID();
			// イベントに応じてシーンを切り替える
			switch (event_id) {
			case UIEventID::Tutorial:
				ErrorMessage(L"未実装");
				break;
			case UIEventID::Play:
				// キャラセレクトシーンに進む
				m_nextSceneID = SceneID::CharaSelect;
				// フェードアウト後にシーン遷移する
				m_fadeScreen->Start();
				m_wasSelected = true;
				// BGMをフェードアウトさせる
				ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Title, 0, 0.9f, 1.0f, 0.0f);
				break;
			case UIEventID::Option:
				ErrorMessage(L"未実装");
				break;
				// ゲームを終了する
			case UIEventID::Exit:
				ExitGame();
				break;
			default:
				ErrorMessage(L"不正なUIイベントを取得しました");
				break;
			}
		}
	}

	// 画面用フェードを更新する
	m_fadeScreen->Update(timer);
	// 画面用フェードの段階を更新する
	if (m_fadeScreen->IsFinished() && m_fadeScreen->IsRunning()) {
		switch (m_fadeScreenStep) {
		// 少し待つ
		case 0:
			m_fadeScreen->Initialize(Fade::State::NoFade, 1.0f, 1.0f);
			break;
		// フェードインする
		case 1:
			m_fadeScreen->Initialize(Fade::State::FadeIn, 1.0f, 0.0f);
			break;
		// フェードアウトの準備をする
		case 2:
			m_fadeScreen->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
			m_fadeScreen->Stop();
			// UIのフェードを開始する
			m_fadeUI->Start();
			break;
		// フェードアウトが完了したらシーン遷移する
		case 3:
			m_pSceneRequest->RequestScene(m_nextSceneID);
			break;
		default:
			m_fadeScreen->Stop();
			break;
		}
		++m_fadeScreenStep;
	}

	// UI用フェードを更新する
	m_fadeUI->Update(timer);
	// UI用フェードの段階を更新する
	if (m_fadeUI->IsFinished() && m_fadeUI->IsRunning()) {
		switch (m_fadeUIStep) {
		// UIをフェードインさせる
		case 0:
			m_fadeUI->Initialize(Fade::State::FadeInAlpha, 1.5f, 1.0f);
			break;
		case 1:
			m_fadeUI->Stop();
		default:
			break;
		}
		++m_fadeUIStep;
	}
}

/// <summary>
/// タイトルシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void TitleScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Title);
	
	// タイトルを描画する
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::Colors::White, 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// UIを描画する
	if (m_fadeUI->GetAlpha() > 0.0f) {
		for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
	}

	// 時間経過でフェードイン・フェードアウトをする
	m_fadeScreen->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// タイトルシーンを終了する
/// </summary>
void TitleScene::Finalize() {
	// UIからオブザーバをデタッチする
	for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Detach(m_uiObserver.get());
	}
	ResourceLoader::Release(ResourceLoaderID::TitleScene);
}

/// <summary>
/// UIを初期化する
/// </summary>
void TitleScene::InitializeUI() {
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));
	
	// UIの生成
	// チュートリアル
	//{
	//	std::unique_ptr<SoundScaleUpUI> tutorial = std::make_unique<SoundScaleUpUI>(
	//		UIEventID::Tutorial, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.4f));
	//	tutorial->SetText(L"Tutorial");
	//	m_menuUIs.emplace_back(std::move(tutorial));
	//}
	// プレイ
	{
		std::unique_ptr<SoundScaleUpUI> play = std::make_unique<SoundScaleUpUI>(
			UIEventID::Play, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.55f));
		play->SetText(L"Play");
		m_menuUIs.emplace_back(std::move(play));
	}
	// オプション
	//{
	//	std::unique_ptr<SoundScaleUpUI> option = std::make_unique<SoundScaleUpUI>(
	//		UIEventID::Option, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.7f));
	//	option->SetText(L"Option");
	//	m_menuUIs.emplace_back(std::move(option));
	//}
	// 終了
	{
		std::unique_ptr<SoundScaleUpUI> exit = std::make_unique<SoundScaleUpUI>(
			UIEventID::Exit, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.85f));
		exit->SetText(L"Exit");
		m_menuUIs.emplace_back(std::move(exit));
	}

	// 共通の処理
	const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
	for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetFont(font);
		(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
		(*itr)->SetTexture(texture);
		(*itr)->FitTextureSize();
		// UIにオブザーバをアタッチする
		(*itr)->Attach(m_uiObserver.get());
	}
}

