#include "TitleScene.h"
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
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	ResourceLoader::Load(ResourceLoaderID::TitleScene);
	m_time = 0.0f;

	InitializeUI();
}

/// <summary>
/// タイトルシーンを更新する
/// </summary>
/// <param name="timer"></param>
void TitleScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_time += elapsed_time;

	// エスケープキーで終了
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		ExitGame();
	}

	// スペースキーを押すか左クリックした場合
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space) ||
		ServiceLocater<MouseWrapper>::Get()->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED ) {
		// タイトルが表示されていなければ、タイマーを進めて表示させる
		if (m_time < 2.0f) {
			m_time = 2.0f;
		}
		
	}

	// UIを更新する
	if (m_time > 2.5f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}
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
			m_pSceneRequest->RequestScene("CharaSelect");
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

/// <summary>
/// タイトルシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void TitleScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Title);
	// 時間経過でフェードインを行う
	float alpha;
	if (m_time <= 2.0f) {
		alpha = std::max(m_time - 1.0f, 0.0f);
	}
	else {
		alpha = 1.0f;
	}
	// タイトルを描画する
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, alpha), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// UIを描画する
	if (m_time > 2.5f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
	}

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"TitleScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// タイトルシーンを終了する
/// </summary>
void TitleScene::Finalize() {
	// UIからオブザーバをデタッチする
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
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
	{
		std::unique_ptr<ScaleUpUI> tutorial = std::make_unique<ScaleUpUI>(
			UIEventID::Tutorial, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.4f));
		tutorial->SetText(L"Tutorial");
		m_menuUIs.emplace_back(std::move(tutorial));
	}
	// プレイ
	{
		std::unique_ptr<ScaleUpUI> play = std::make_unique<ScaleUpUI>(
			UIEventID::Play, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.55f));
		play->SetText(L"Play");
		m_menuUIs.emplace_back(std::move(play));
	}
	// オプション
	{
		std::unique_ptr<ScaleUpUI> option = std::make_unique<ScaleUpUI>(
			UIEventID::Option, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.7f));
		option->SetText(L"Option");
		m_menuUIs.emplace_back(std::move(option));
	}
	// 終了
	{
		std::unique_ptr<ScaleUpUI> exit = std::make_unique<ScaleUpUI>(
			UIEventID::Exit, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.85f));
		exit->SetText(L"Exit");
		m_menuUIs.emplace_back(std::move(exit));
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

