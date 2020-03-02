#include "CharaSelectScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\UISubject.h>
#include <Utils\RandMt.h>
#include <Utils\MouseWrapper.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\Scene\ShareData\ShareData.h>
#include <Game\UI\CharaIcon.h>
#include <Game\UI\CharaSelectMarker.h>
#include <Game\UI\MenuUI.h>
#include <Game\UI\Fade.h>


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

	// リソースのロード
	ResourceLoader::Load(ResourceLoaderID::CharaSelectScene);

	m_state = CharaSelectState::SelectPlayer;
	m_currentPlayer = 0;
	m_selectCharaId.resize(PLAYER_COUNT);
	// UIを初期化する
	InitializeUI();
	// フェードを生成する
	m_fade = std::make_unique<Fade>();
	m_fade->Initialize(Fade::State::FadeIn, 1.0f, 0.0f);

	// BGMを再生する
	ServiceLocater<AudioManager>::Get()->PlayBgm(BgmID::CharaSelect);
	// BGMをフェードインさせる
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::CharaSelect, 0, 0.9f, 0.0f, 1.0f);
}

/// <summary>
/// キャラセレクトシーンを更新する
/// </summary>
/// <param name="timer"></param>
void CharaSelectScene::Update(const DX::StepTimer& timer) {
	// エスケープキーで終了
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		ExitGame();
	}

	// フェードを更新する
	m_fade->Update(timer);
	// フェードが完了するまで処理しない
	if (!m_fade->IsFinished()) {
		return;
	}

	// UIを更新する
	if (m_state != CharaSelectScene::CharaSelectState::FadeOut) {
		// 戻る・進むボタン
		for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}
		// キャラアイコン
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Update(timer);
		}
	}

	// ステートに応じた更新処理を行う
	switch (m_state) {
	case CharaSelectScene::CharaSelectState::SelectPlayer:
		UpdateSelectPlayer(timer);
		break;
	case CharaSelectScene::CharaSelectState::Ready:
		UpdateReady(timer);
		break;
	case CharaSelectScene::CharaSelectState::FadeOut:
		UpdateFadeOut(timer);
		break;
	default:
		ErrorMessage(L"キャラセレクトで不正な状態が渡されました");
		break;
	}

}

void CharaSelectScene::UpdateSelectPlayer(const DX::StepTimer& timer) {
	timer;

	// ランダム用変数
	int rand;
	// 次に進んだかどうか
	bool select_next = false;

	// 右クリックでキャラマニュアルを呼び出す
	const bool right_click = (ServiceLocater<MouseWrapper>::Get()->GetTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED);
	if (right_click) {
		m_pSceneRequest->RequestScene(SceneID::CharaManual, RequestSceneType::StackScene);
		return;
	}

	// イベントを取得しているかどうか確認する
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
			
		case UIEventID::Back:
			// タイトルに戻る
			if (m_currentPlayer == 0) {
				// フェードアウト後にシーン遷移する
				m_state = CharaSelectState::FadeOut;
				m_nextSceneID = SceneID::Title;
				m_fade->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
			}
			// 前のキャラクターの選択に戻る
			else {
				// 選択済みキャラクターを隠す
				m_backCharas[m_currentPlayer - 1]->SetTexture(nullptr);
				// 操作するマーカーを切り替える
				m_markerUIs[m_currentPlayer]->SetEnableObserver(false);
				m_markerUIs[m_currentPlayer - 1]->SetEnableObserver(true);
				--m_currentPlayer;
			}
			break;
		case UIEventID::Next:
			// ランダムでキャラを決める
			rand = RandMt::GetRand(static_cast<int>(m_charaIcons.size()));
			SelectChara(m_charaIcons[rand].get(), m_backCharas[m_currentPlayer].get(), m_markerUIs[m_currentPlayer].get());
			select_next = true;
			break;
			// キャラクターを選択する
		case UIEventID::CharaIcon:
			SelectChara(ui_event.address, m_backCharas[m_currentPlayer].get(), m_markerUIs[m_currentPlayer].get());
			select_next = true;
			break;
		}
	}

	// 次に進んだ場合
	if (select_next) {
		// 次のキャラクター選択に進む
		if (m_currentPlayer != PLAYER_COUNT - 1) {
			// 操作するマーカーを切り替える
			m_markerUIs[m_currentPlayer]->SetEnableObserver(false);
			m_markerUIs[m_currentPlayer + 1]->SetEnableObserver(true);
			++m_currentPlayer;
		}
		// 最後のキャラクターを選択した場合
		else {
			// マーカーの操作を解除する
			m_markerUIs[m_currentPlayer]->SetEnableObserver(false);
			// 次に進むボタンのテキストを変更する
			m_menuUIs[1]->SetText(L"Fight!");
			// UIの効果音を変更する
			m_menuUIs[1]->SetOnClickSound(SoundID::Ready);
			// 決定待ち状態に進む
			m_state = CharaSelectState::Ready;
		}
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
			// キャラ選択に戻る
		case UIEventID::Back:
			m_backCharas[2]->SetTexture(nullptr);
			m_markerUIs[m_currentPlayer]->SetEnableObserver(true);
			// 次に進むボタンを元に戻す
			m_menuUIs[1]->SetText(L"Random");
			m_menuUIs[1]->SetOnClickSound(SoundID::Decision);
			m_state = CharaSelectState::SelectPlayer;
			break;
			// キャラを確定する
		case UIEventID::Next:
			// 選択したキャラのIDを保持する
			ServiceLocater<ShareData>::Get()->SetSelectCharaID(m_selectCharaId);
			// フェードアウト後にシーン遷移する
			m_state = CharaSelectState::FadeOut;
			m_nextSceneID = SceneID::Play;
			m_fade->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
			// BGMをフェードアウトさせる
			ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::CharaSelect, 0, 0.9f, 1.0f, 0.0f);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// フェードアウト状態
/// </summary>
/// <param name="timer">ステップタイマー</param>
void CharaSelectScene::UpdateFadeOut(const DX::StepTimer& timer) {
	timer;

	// ここに処理が到達する段階でフェードアウトが完了しているため、そのままシーン遷移する
	m_pSceneRequest->RequestScene(m_nextSceneID);
}

/// <summary>
/// キャラを選択する
/// </summary>
/// <param name="charaIcon">キャラアイコン</param>
/// <param name="backChara">選択済みキャラ</param>
/// <param name="marker">選択マーカー</param>
void CharaSelectScene::SelectChara(const UISubject* charaIcon, UISubject* backChara, CharaSelectMarker* marker) {
	// 選択済みキャラにテクスチャをセットする
	backChara->SetTexture(ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaPortrait2));
	backChara->SetTextureIndex(charaIcon->GetTextureIndex());
	backChara->FitTextureSize();
	// ランダムで選択した場合のために、マーカーを動かす
	marker->SetPos(charaIcon->GetPos());
	// 選択したキャラのIDを保存する
	m_selectCharaId[m_currentPlayer] = charaIcon->GetTextureIndex();
}


/// <summary>
/// キャラセレクトシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void CharaSelectScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaSelectBackGround);

	// 背景を描画する
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::Colors::White, 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// マニュアルテキストを描画する
	texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaManualText);
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.95f),
		nullptr, DirectX::Colors::White, 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.6f, 0.6f));

	// UIを描画する
	// 戻る・進むボタン
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
	// 選択済みキャラ
	for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_backCharas.begin(); itr != m_backCharas.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
	// キャラアイコン
	for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
	// 選択マーカー
	for (int i = 0; i <= m_currentPlayer; ++i) {
		m_markerUIs[i]->Render(spriteBatch);
	}

	// 時間経過でフェードイン・フェードアウトする
	m_fade->Render(spriteBatch);

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
		std::unique_ptr<MenuUI> back = std::make_unique<MenuUI>(
			UIEventID::Back, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.1f, screen_size.y*0.9f));
		back->SetText(L"Back");
		m_menuUIs.emplace_back(std::move(back));
		// 進むボタン
		std::unique_ptr<MenuUI> next = std::make_unique<MenuUI>(
			UIEventID::Next, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.9f, screen_size.y*0.9f));
		next->SetText(L"Random");
		m_menuUIs.emplace_back(std::move(next));
		// ボタンの共通処理
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

	// キャラアイコンの生成
	{
		// キャラクターの数
		constexpr int chara_count = 3;
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaPortrait);
		constexpr float scale = 0.6f;
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
		constexpr float scale = 0.9f;
		for (int i = 0; i < PLAYER_COUNT; ++i) {
			std::unique_ptr<UISubject> chara = std::make_unique<UISubject>(
				UIEventID::Null, 0, DirectX::SimpleMath::Vector2(screen_size.x*(0.2f+0.3f*i), screen_size.y*0.4f));
			chara->SetScale(DirectX::SimpleMath::Vector2::One*scale);
			m_backCharas.emplace_back(std::move(chara));
		}
	}
	// 選択マーカー
	{
		constexpr float scale = 0.6f;
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaSelectMarker);
		for (int i = 0; i < PLAYER_COUNT; ++i) {
			std::unique_ptr<CharaSelectMarker> marker = std::make_unique<CharaSelectMarker>(
				0, DirectX::SimpleMath::Vector2::Zero, texture, i);
			if (i == 0) {
				// 最初のマーカーだけ左に配置する
				marker->SetPos(m_charaIcons.front()->GetPos());
			}
			else {
				marker->SetPos(m_charaIcons.front()->GetPos());
				// 他のマーカーの位置を固定する
				marker->SetEnableObserver(false);
			}
			marker->SetScale(DirectX::SimpleMath::Vector2::One*scale);
			m_markerUIs.emplace_back(std::move(marker));

			// キャラアイコンにマーカーをアタッチする
			for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
				(*itr)->Attach(m_markerUIs[i].get());
			}
		}
	}

}


