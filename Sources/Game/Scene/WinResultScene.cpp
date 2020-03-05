#include "WinResultScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\MouseWrapper.h>
#include "ISceneRequest.h"
#include <Game\UI\MenuUI.h>
#include <Game\UI\Fade.h>


/// <summary>
/// コンストラクタ
/// </summary>
WinResultScene::WinResultScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
WinResultScene::~WinResultScene() {
}

/// <summary>
///	勝利リザルトシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void WinResultScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	// UIを初期化する
	InitializeUI();

	// マウスカーソルを表示する
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);

	// フェードを生成する
	m_fadeScreen = std::make_unique<Fade>();
	m_fadeScreen->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
	// 後から動作を開始させる
	m_fadeScreen->Stop();

	m_fadeLogo = std::make_unique<Fade>();
	m_fadeLogo->Initialize(Fade::State::FadeInAlpha, 1.0f, 1.0f);

	m_fadeUI = std::make_unique<Fade>();
	m_fadeUI->Initialize(Fade::State::FadeInAlpha, 1.0f, 1.0f);
	// 後から動作を開始させる
	m_fadeUI->Stop();
	m_fadeUIFinished = false;

	m_wasSelected = false;
}

/// <summary>
/// 勝利リザルトシーンを更新する
/// </summary>
/// <param name="timer"></param>
void WinResultScene::Update(const DX::StepTimer& timer) {

	// フェードを更新する
	m_fadeScreen->Update(timer);
	m_fadeLogo->Update(timer);
	m_fadeUI->Update(timer);

	// リザルトロゴのフェードが完了したらUIのフェードを開始させる
	if (m_fadeLogo->IsFinished() && !m_fadeUI->IsRunning()) {
		m_fadeUI->Start();
	}

	// UIのアルファ値を更新する
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetAlpha(m_fadeUI->GetAlpha());
	}

	// UIのフェードが完了したタイミングでカーソルを初期位置にする
	if (!m_fadeUIFinished && m_fadeUI->IsFinished()) {
		m_menuUIs[m_selectedUI]->Select();
		m_fadeUIFinished = true;
	}

	// 未選択でフェードが完了していたらUIを更新する
	if (!m_wasSelected && m_fadeUI->IsFinished()) {
		// パッド接続時にカーソル移動を行う
		const InputManager* input_manager = ServiceLocater<InputManager>::Get();
		if (input_manager->IsPadConnected()) {
			if (input_manager->IsPressed(InputID::Left)) {
				m_menuUIs[m_selectedUI]->Deselect();
				m_selectedUI = (m_selectedUI + m_menuUIs.size() - 1) % m_menuUIs.size();
				m_menuUIs[m_selectedUI]->Select();
			}
			else if (input_manager->IsPressed(InputID::Right)) {
				m_menuUIs[m_selectedUI]->Deselect();
				m_selectedUI = (m_selectedUI + 1) % m_menuUIs.size();
				m_menuUIs[m_selectedUI]->Select();
			}
		}

		for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}

		// イベントを取得しているかどうか確認する
		if (m_uiObserver->HasNewEvent()) {
			UIEventID event_id = m_uiObserver->GetEventID();
			// イベントに応じてシーンを切り替える
			switch (event_id) {
				// キャラセレクトに戻る
			case UIEventID::CharaSelect:
				m_nextSceneID = SceneID::CharaSelect;
				m_fadeScreen->Start();
				m_wasSelected = true;
				break;
				// タイトルに戻る
			case UIEventID::Title:
				m_nextSceneID = SceneID::Title;
				m_fadeScreen->Start();
				m_wasSelected = true;
				break;
			default:
				ErrorMessage(L"不正なUIイベントを取得しました");
				break;
			}
		}
	}

	// フェードアウトが完了したらシーン遷移する
	if (m_fadeScreen->IsFinished()) {
		m_pSceneRequest->RequestScene(m_nextSceneID);
	}

}

/// <summary>
/// 勝利リザルトシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void WinResultScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	// 勝利ロゴを描画する
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Player1Win);
	DirectX::SimpleMath::Vector2 pos(ServiceLocater<DirectX11>::Get()->GetWidth()*0.5f, 100.0f);
	spriteBatch->Draw(texture->GetResource().Get(), pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, m_fadeLogo->GetAlpha()), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2::One);

	// UIを描画する
	for (std::vector<std::unique_ptr<MenuUI>>::const_iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}


	// 時間経過でフェードアウトを行う
	m_fadeScreen->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// 勝利リザルトシーンを終了する
/// </summary>
void WinResultScene::Finalize() {

}

/// <summary>
/// UIを初期化する
/// </summary>
void WinResultScene::InitializeUI() {
	// オブザーバを生成する
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UIの生成
	// キャラセレクト
	{
		std::unique_ptr<MenuUI> charaselect = std::make_unique<MenuUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.3f, screen_size.y*0.8f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// タイトル 
	{
		std::unique_ptr<MenuUI> title = std::make_unique<MenuUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.7f, screen_size.y*0.8f));
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

	m_selectedUI = 0;
}

