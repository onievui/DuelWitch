#include "PauseScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\ScaleUpUI.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"


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
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	// UIを初期化する
	InitializeUI();

	// マウスカーソルを表示する
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
}

/// <summary>
/// ポーズシーンを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void PauseScene::Update(const DX::StepTimer& timer) {
	timer;

	// エスケープキーを押して、プレイシーンを再開する
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		Resume();
		return;
	}

	// UIを更新する
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// イベントを取得しているかどうか確認する
	if (m_uiObserver->HasNewEvent()) {
		UIEventID event_id = m_uiObserver->GetEventID();
		// イベントに応じてシーンを切り替える
		switch (event_id) {
		// 再開する
		case UIEventID::Resume:
			Resume();
			return;
		// キャラセレクトに戻る
		case UIEventID::CharaSelect:
			m_pSceneRequest->RequestScene("CharaSelect");
			break;
		// タイトルに戻る
		case UIEventID::Title:
			m_pSceneRequest->RequestScene("Title");
			break;
		default:
			ErrorMessage(L"不正なUIイベントを取得しました");
			break;
		}
	}

}

/// <summary>
/// ポーズシーンを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void PauseScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	// プレイ画面に黒を重ねて暗くする
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::BlackBack);
	spriteBatch->Draw(texture->GetResource().Get(), DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Color(1, 1, 1, 0.8f));

	// UIを描画する
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
	

	spriteBatch->End();
}

/// <summary>
/// ポーズシーンを終了する
/// </summary>
void PauseScene::Finalize() {
	// UIからオブザーバをデタッチする
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
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
		std::unique_ptr<ScaleUpUI> resume = std::make_unique<ScaleUpUI>(
			UIEventID::Resume, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.25f));
		resume->SetText(L"Resume");
		m_menuUIs.emplace_back(std::move(resume));
	}
	// キャラセレクト
	{
		std::unique_ptr<ScaleUpUI> charaselect = std::make_unique<ScaleUpUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.5f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// タイトル 
	{
		std::unique_ptr<ScaleUpUI> title = std::make_unique<ScaleUpUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.75f));
		title->SetText(L"Title");
		m_menuUIs.emplace_back(std::move(title));
	}
	
	// 共通の処理
	const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetFont(font);
		(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
		(*itr)->SetTexture(texture);
		(*itr)->FitTextureSize();
		// UIにオブザーバをアタッチする
		(*itr)->Attach(m_uiObserver.get());
	}
}

/// <summary>
/// ポーズを解除する
/// </summary>
void PauseScene::Resume() {
	m_pSceneRequest->PopScene();
	// マウスカーソルを非表示に戻す
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
}

