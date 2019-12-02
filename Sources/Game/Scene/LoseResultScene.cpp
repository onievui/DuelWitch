#include "LoseResultScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\ScaleUpUI.h>
#include <Utils\MouseWrapper.h>
#include "ISceneRequest.h"


/// <summary>
/// コンストラクタ
/// </summary>
LoseResultScene::LoseResultScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
LoseResultScene::~LoseResultScene() {
}

/// <summary>
///	敗北リザルトシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void LoseResultScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	m_time = 0.0f;

	// UIを初期化する
	InitializeUI();

	// マウスカーソルを表示する
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
}

/// <summary>
/// 敗北リザルトシーンを更新する
/// </summary>
/// <param name="timer"></param>
void LoseResultScene::Update(const DX::StepTimer& timer) {
	float elapesd_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapesd_time;

	// UIを更新する
	if (m_time >= 3.0f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}
	}

	// イベントを取得しているかどうか確認する
	if (m_uiObserver->HasNewEvent()) {
		UIEventID event_id = m_uiObserver->GetEventID();
		// イベントに応じてシーンを切り替える
		switch (event_id) {
			// キャラセレクトに戻る
		case UIEventID::CharaSelect:
			m_pSceneRequest->RequestScene(SceneID::CharaSelect);
			break;
			// タイトルに戻る
		case UIEventID::Title:
			m_pSceneRequest->RequestScene(SceneID::Title);
			break;
		default:
			ErrorMessage(L"不正なUIイベントを取得しました");
			break;
		}
	}
}

/// <summary>
/// 敗北リザルトシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void LoseResultScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	// 時間経過でフェードアウトを行う
	float alpha;
	if (m_time <= 3.0f) {
		alpha = m_time / 3.0f*0.8f;
	}
	else {
		alpha = 0.8f;
	}

	// プレイ画面に黒を重ねて暗くする
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Fade);
	spriteBatch->Draw(texture->GetResource().Get(), DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Color(0, 0, 0, alpha));

	// 敗北テクスチャを描画する
	if (m_time >= 3.0f) {		
		texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::YouLose);
		// 時間経過でフェードインを行う
		alpha = std::min((m_time - 3.0f) / 4.0f, 1.0f);
		DirectX::SimpleMath::Vector2 pos(ServiceLocater<DirectX11>::Get()->GetWidth()*0.5f, 100.0f);
		spriteBatch->Draw(texture->GetResource().Get(), pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, alpha), 0,
			texture->GetCenter(), DirectX::SimpleMath::Vector2::One);
	}

	// UIを描画する
	if (m_time >= 4.0f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::const_iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
	}


	spriteBatch->End();
}

/// <summary>
/// リザルトシーンを終了する
/// </summary>
void LoseResultScene::Finalize() {

}

/// <summary>
/// UIを初期化する
/// </summary>
void LoseResultScene::InitializeUI() {
	// オブザーバを生成する
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UIの生成
	// キャラセレクト
	{
		std::unique_ptr<ScaleUpUI> charaselect = std::make_unique<ScaleUpUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.3f, screen_size.y*0.8f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// タイトル 
	{
		std::unique_ptr<ScaleUpUI> title = std::make_unique<ScaleUpUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.7f, screen_size.y*0.8f));
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

