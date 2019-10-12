#include "CharaSelectScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\UISubject.h>
#include <Utils\RandMt.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"
#include "CharaIcon.h"
#include "CharaSelectMarker.h"


/// <summary>
/// コンストラクタ
/// </summary>
CharaSelectScene::CharaSelectScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
CharaSelectScene::~CharaSelectScene() {
}

/// <summary>
///	キャラセレクトシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void CharaSelectScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	// リソースのロード
	ResourceLoader::Load(ResourceLoaderID::CharaSelectScene);

	m_time = 0.0f;
	m_state = CharaSelectState::SelectPlayer1;
	// UIを初期化する
	InitializeUI();
}

/// <summary>
/// キャラセレクトシーンを更新する
/// </summary>
/// <param name="timer"></param>
void CharaSelectScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_time += elapsed_time;

	// エスケープキーで終了
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		ExitGame();
	}

	// UIを更新する
	// 戻る・進むボタン
	for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Update(timer);
	}
	// キャラアイコン
	for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// ステートに応じた更新処理を行う
	switch (m_state) {
	case CharaSelectScene::CharaSelectState::SelectPlayer1:
		UpdateSelectPlayer1(timer);
		break;
	case CharaSelectScene::CharaSelectState::SelectPlayer2:
		UpdateSelectPlayer2(timer);
		break;
	case CharaSelectScene::CharaSelectState::Ready:
		UpdateReady(timer);
		break;
	default:
		ErrorMessage(L"キャラセレクトで不正な状態が渡されました");
		break;
	}

}

/// <summary>
/// プレイヤー1のキャラクターを選択する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void CharaSelectScene::UpdateSelectPlayer1(const DX::StepTimer& timer) {
	timer;
	// ランダム用変数
	int rand;
	// イベントを取得しているかどうか確認する
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
			// タイトルに戻る
		case UIEventID::Back:
			m_pSceneRequest->RequestScene("Title");
			break;
			// ランダムでキャラを決める
		case UIEventID::Next:
			rand = RandMt::GetRand(static_cast<int>(m_charaIcons.size()));
			SelectChara(m_charaIcons[rand].get(), m_backCharas[0].get(), m_markerUIs[0].get());
			m_state = CharaSelectState::SelectPlayer2;
			break;
			// キャラクターを選択する
		case UIEventID::CharaIcon:
			SelectChara(ui_event.address, m_backCharas[0].get(), m_markerUIs[0].get());
			m_state = CharaSelectState::SelectPlayer2;
			break;
		default:
			break;
		}
	}

	// 次に進む場合
	if (m_state == CharaSelectState::SelectPlayer2) {
		// キャラアイコンにアタッチするマーカーを変更する
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Detach(m_markerUIs[0].get());
			(*itr)->Attach(m_markerUIs[1].get());
		}
	}
}

/// <summary>
/// プレイヤー2のキャラクターを選択する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void CharaSelectScene::UpdateSelectPlayer2(const DX::StepTimer& timer) {
	timer;
	// ランダム用変数
	int rand;
	// イベントを取得しているかどうか確認する
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
		// プレイヤー1のキャラ選択に戻る
		case UIEventID::Back:
			m_backCharas[0]->SetTexture(nullptr);
			m_state = CharaSelectState::SelectPlayer1;
			break;
		// ランダムでキャラを決める
		case UIEventID::Next:
			rand = RandMt::GetRand(static_cast<int>(m_charaIcons.size()));
			SelectChara(m_charaIcons[rand].get(), m_backCharas[1].get(), m_markerUIs[1].get());
			m_state = CharaSelectState::Ready;
			break;
		// キャラクターを選択する
		case UIEventID::CharaIcon:
			SelectChara(ui_event.address, m_backCharas[1].get(), m_markerUIs[1].get());
			m_state = CharaSelectState::Ready;
			break;
		default:
			break;
		}
	}

	// 前に戻る場合
	if (m_state == CharaSelectState::SelectPlayer1) {
		// キャラアイコンにアタッチするマーカーを変更する
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Detach(m_markerUIs[1].get());
			(*itr)->Attach(m_markerUIs[0].get());
		}
	}
	// 次に進む場合
	if (m_state == CharaSelectState::Ready) {
		// キャラアイコンにアタッチするマーカーを変更する
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Detach(m_markerUIs[1].get());
		}
		m_menuUIs[1]->SetText(L"Fight!");
	}
}

/// <summary>
/// 決定待ち状態
/// </summary>
/// <param name="timer">ステップタイマー</param>
void CharaSelectScene::UpdateReady(const DX::StepTimer& timer) {
	timer;
	// イベントを取得しているかどうか確認する
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
			// プレイヤー2のキャラ選択に戻る
		case UIEventID::Back:
			m_backCharas[1]->SetTexture(nullptr);
			m_state = CharaSelectState::SelectPlayer2;
			break;
			// キャラを確定する
		case UIEventID::Next:
			m_pSceneRequest->RequestScene("Play");
			break;
		default:
			break;
		}
	}

	// 前に戻る場合
	if (m_state == CharaSelectState::SelectPlayer1) {
		// キャラアイコンにアタッチするマーカーを変更する
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Attach(m_markerUIs[1].get());
		}
	}
}

/// <summary>
/// キャラを選択する
/// </summary>
/// <param name="charaIcon">キャラアイコン</param>
/// <param name="backChara">選択済みキャラ</param>
/// <param name="marker">選択マーカー</param>
void CharaSelectScene::SelectChara(const UISubject* charaIcon, UISubject* backChara, CharaSelectMarker* marker) {
	// 選択済みキャラにテクスチャをセットする
	backChara->SetTexture(charaIcon->GetTexture());
	backChara->SetTextureIndex(charaIcon->GetTextureIndex());
	backChara->FitTextureSize();
	// ランダムで選択した場合のために、マーカーを動かす
	marker->SetPos(charaIcon->GetPos());
}


/// <summary>
/// キャラセレクトシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void CharaSelectScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaSelectBackGround);
	// 時間経過でフェードインを行う
	float alpha = std::min(m_time, 1.0f);
	// 背景を描画する
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, alpha), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// UIを描画する
	if (m_time > 1.0f) {
		// 戻る・進むボタン
		for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
		// キャラアイコン
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
		// 選択済みキャラ
		for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_backCharas.begin(); itr != m_backCharas.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
		// 選択マーカー
		m_markerUIs[0]->Render(spriteBatch);
		if (m_state != CharaSelectState::SelectPlayer1) {
			m_markerUIs[1]->Render(spriteBatch);
		}
	}

	spriteBatch->End();
}

/// <summary>
/// キャラセレクトシーンを終了する
/// </summary>
void CharaSelectScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::CharaSelectScene);
}

/// <summary>
/// UIを初期化する
/// </summary>
void CharaSelectScene::InitializeUI() {
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UIの生成
	// ボタン
	{
		// 戻るボタン
		std::unique_ptr<UISubject> back = std::make_unique<UISubject>(
			UIEventID::Back, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.1f, screen_size.y*0.9f));
		back->SetText(L"Back");
		m_menuUIs.emplace_back(std::move(back));
		// 進むボタン
		std::unique_ptr<UISubject> next = std::make_unique<UISubject>(
			UIEventID::Next, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.9f, screen_size.y*0.9f));
		next->SetText(L"Random");
		m_menuUIs.emplace_back(std::move(next));
		// ボタンの共通処理
		const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
		for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->SetFont(font);
			(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
			(*itr)->SetTexture(texture);
			(*itr)->FitTextureSize();
			// UIにオブザーバをアタッチする
			(*itr)->Attach(m_uiObserver.get());
		}
	}

	// キャラアイコンの生成
	{
		// キャラアイコンの数
		constexpr int chara_count = 3;
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaIcon);
		float scale = 0.6f;
		float texture_width = texture->GetSize().x*scale;
		for (int i = 0; i < chara_count; ++i) {
			// キャラアイコンを並べるためのオフセット
			float x = (i - (chara_count - 1)*0.5f)*texture_width;
			std::unique_ptr<CharaIcon> chara_icon = std::make_unique<CharaIcon>(
				0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f+x, screen_size.y*0.75f), texture, i);
			chara_icon->SetScale(DirectX::SimpleMath::Vector2::One*scale);
			chara_icon->Attach(m_uiObserver.get());
			m_charaIcons.emplace_back(std::move(chara_icon));
		}
	}
	// 選択済みキャラクター
	{
		// プレイヤー1キャラクター
		std::unique_ptr<UISubject> chara1 = std::make_unique<UISubject>(
			UIEventID::Null, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.2f, screen_size.y*0.4f));
		m_backCharas.emplace_back(std::move(chara1));
		// プレイヤー2キャラクター
		std::unique_ptr<UISubject> chara2 = std::make_unique<UISubject>(
			UIEventID::Null, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.8f, screen_size.y*0.4f));
		m_backCharas.emplace_back(std::move(chara2));
	}
	// 選択マーカー
	{
		float scale = 0.6f;
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaSelectMarker);
		// プレイヤー1選択マーカー
		std::unique_ptr<CharaSelectMarker> marker1 = std::make_unique<CharaSelectMarker>(
			0, m_charaIcons.front()->GetPos(), texture, 0);
		marker1->SetScale(DirectX::SimpleMath::Vector2::One*scale);
		m_markerUIs.emplace_back(std::move(marker1));
		// プレイヤー2選択マーカー
		std::unique_ptr<CharaSelectMarker> marker2 = std::make_unique<CharaSelectMarker>(
			0, m_charaIcons.back()->GetPos(), texture, 1);
		marker2->SetScale(DirectX::SimpleMath::Vector2::One*scale);
		m_markerUIs.emplace_back(std::move(marker2));

		// キャラアイコンにマーカーをアタッチする
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Attach(m_markerUIs[0].get());
		}
	}

}

