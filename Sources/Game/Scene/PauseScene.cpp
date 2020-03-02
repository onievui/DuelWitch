#include "PauseScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\LoadDataManager.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\MenuUI.h>
#include <Game\UI\Fade.h>


/// <summary>
/// コンストラクタ
/// </summary>
PauseScene::PauseScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
PauseScene::~PauseScene() {
}

/// <summary>
///	ポーズシーンを初期化する
/// </summary>
/// <param name="pSceneRequest">リクエストシーンインタフェース</param>
void PauseScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	// UIを初期化する
	InitializeUI();

	// マウスカーソルを表示する
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
	ServiceLocater<MouseWrapper>::Get()->SetVisible(true);
	// マウスをウインドウ内に移動できるようにする
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(false);

	// フェードを生成する
	m_fadeBack = std::make_unique<Fade>();
	m_fadeBack->Initialize(Fade::State::FadeOut, PAUSE_TIME, 0.8f);

	m_fadeFront = std::make_unique<Fade>();
	m_fadeFront->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
	// 後から動作を開始させる
	m_fadeFront->Stop();

	m_fadeUI = std::make_unique<Fade>();
	m_fadeUI->Initialize(Fade::State::FadeInAlpha, PAUSE_TIME, 1.0f);

	m_wasSelected = false;

	// ポーズ音を鳴らす
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Pause);

	// BGMの音量を下げる
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, PAUSE_TIME, 1.0f, BGM_VOLUME);

}

/// <summary>
/// ポーズシーンを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void PauseScene::Update(const DX::StepTimer& timer) {
	
	m_fadeBack->Update(timer);
	m_fadeFront->Update(timer);
	m_fadeUI->Update(timer);

	// UIのアルファ値を更新する
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetAlpha(m_fadeUI->GetAlpha());
	}

	// フェードが完了するまで処理しない
	if (!m_fadeBack->IsFinished()) {
		return;
	}

	// F2キーでパラメータを再読み込みする
	const bool press_f2 = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F2);
	if (press_f2) {
		LoadDataManager::GetIns()->Reload(LoadDataID::PlayScene);
	}

	// エスケープキーを押して、プレイシーンを再開する
	const bool press_escape = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape);
	if (press_escape) {
		SelectResume();
	}

	// 未選択でフェードが完了していたらUIを更新する
	if (!m_wasSelected && m_fadeUI->IsFinished()) {
		// UIを更新する
		for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}

		// イベントを取得しているかどうか確認する
		if (m_uiObserver->HasNewEvent()) {
			UIEventID event_id = m_uiObserver->GetEventID();
			// イベントに応じてシーンを切り替える
			switch (event_id) {
				// 再開する
			case UIEventID::Resume:
				SelectResume();
				return;
				// キャラセレクトに戻る
			case UIEventID::CharaSelect:
				m_nextSceneID = SceneID::CharaSelect;
				m_fadeFront->Start();
				// BGMをフェードアウトさせる
				ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, BGM_VOLUME, 0.0f);
				break;
				// タイトルに戻る
			case UIEventID::Title:
				m_nextSceneID = SceneID::Title;
				m_fadeFront->Start();
				// BGMをフェードアウトさせる
				ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, BGM_VOLUME, 0.0f);
				break;
			default:
				ErrorMessage(L"不正なUIイベントを取得しました");
				break;
			}
			m_wasSelected = true;
		}
	}

	// 選択してから一定時間経過したらシーンを遷移する
	if (m_fadeFront->IsFinished()) {
		//プレイシーンを選択していたらポーズを解除する
		if (m_nextSceneID == SceneID::Play) {
			Resume();
		}
		// それ以外なら、そのシーンに遷移する
		else {
			m_pSceneRequest->RequestScene(m_nextSceneID);
		}
	}

}

/// <summary>
/// ポーズシーンを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void PauseScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	// プレイ画面に黒を重ねて暗くする
	m_fadeBack->Render(spriteBatch);

	// UIを描画する
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
	
	// 再開しない場合はフェードアウトする
	m_fadeFront->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// ポーズシーンを終了する
/// </summary>
void PauseScene::Finalize() {
	// UIからオブザーバをデタッチする
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Detach(m_uiObserver.get());
	}
}

/// <summary>
/// UIを初期化する
/// </summary>
void PauseScene::InitializeUI() {
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UIの生成
	// 再開
	{
		std::unique_ptr<MenuUI> resume = std::make_unique<MenuUI>(
			UIEventID::Resume, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.25f));
		resume->SetText(L"Resume");
		m_menuUIs.emplace_back(std::move(resume));
	}
	// キャラセレクト
	{
		std::unique_ptr<MenuUI> charaselect = std::make_unique<MenuUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.5f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// タイトル 
	{
		std::unique_ptr<MenuUI> title = std::make_unique<MenuUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.75f));
		title->SetText(L"Title");
		m_menuUIs.emplace_back(std::move(title));
	}
	
	// 共通の処理
	const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetFont(font);
		(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
		(*itr)->SetTexture(texture);
		(*itr)->FitTextureSize();
		// UIにオブザーバをアタッチする
		(*itr)->Attach(m_uiObserver.get());
	}
}

/// <summary>
/// ポーズの解除を選択する
/// </summary>
void PauseScene::SelectResume() {
	// アルファ値が0.8から始まるように調整する
	m_fadeBack->Initialize(Fade::State::FadeIn, PAUSE_TIME*1.25f, 0.0f);
	m_fadeBack->SkipTime(0.2f);

	m_fadeUI->Initialize(Fade::State::FadeOutAlpha, PAUSE_TIME, 0.0f);

	// 終了判定のために手前画面用フェードをタイマーとして扱う
	m_fadeFront->Initialize(Fade::State::NoFade, PAUSE_TIME, 0.0f);

	m_wasSelected = true;
	m_nextSceneID = SceneID::Play;

	// BGMの音量を戻す
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, PAUSE_TIME, BGM_VOLUME, 1.0f);
}

/// <summary>
/// ポーズを解除する
/// </summary>
void PauseScene::Resume() {
	m_pSceneRequest->PopScene();
	// マウスカーソルを非表示に戻す
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
	ServiceLocater<MouseWrapper>::Get()->SetVisible(false);
	// マウスをウインドウ内に収める
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(true);
}

