#pragma once
#ifndef CHARA_SELECT_SCENE_DEFINED
#define CHARA_SELECT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class UISubject;
class MenuUI;
class CharaIcon;
class CharaSelectMarker;
class Fade;
enum class SceneID;


/// <summary>
/// キャラセレクトシーンクラス
/// </summary>
class CharaSelectScene : public IScene {
private:
	// キャラセレクトシーンステート
	enum class CharaSelectState {
		SelectPlayer,
		Ready,
		FadeOut,
	};

	// 選択するキャラの数
	static constexpr int PLAYER_COUNT = 3;

public:
	// コンストラクタ
	CharaSelectScene();
	// デストラクタ
	~CharaSelectScene();
public:
	// キャラセレクトシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// キャラセレクトシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// キャラセレクトシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// キャラセレクトシーンを終了する
	void Finalize() override;

private:
	// UIを初期化する
	void InitializeUI();
	// キャラクターを選択する
	void UpdateSelectPlayer(const DX::StepTimer& timer);
	// 決定待ち状態
	void UpdateReady(const DX::StepTimer& timer);
	// フェードアウト状態
	void UpdateFadeOut(const DX::StepTimer& timer);
	// キャラを選択する
	void SelectChara(const UISubject* charaIcon, UISubject* backChara, CharaSelectMarker* marker);

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                   m_pSceneRequest;

	// ステート
	CharaSelectState                                 m_state;
	// 選択中のキャラのID
	int                                              m_currentPlayer;
	// 選択したキャラのID
	std::vector<int>                                 m_selectCharaId;

	// 画面用フェード
	std::unique_ptr<Fade>                            m_fade;

	// UIオブザーバ
	std::unique_ptr<UIObserver>                      m_uiObserver;
	// キャラアイコンUI
	std::vector<std::unique_ptr<CharaIcon>>          m_charaIcons;
	// 選択済みキャラクターUI
	std::vector<std::unique_ptr<UISubject>>          m_backCharas;
	// 選択マーカーUI
	std::vector<std::unique_ptr<CharaSelectMarker>>	 m_markerUIs;
	// メニューUI
	std::vector<std::unique_ptr<MenuUI>>     m_menuUIs;

	// 遷移先のシーンID
	SceneID                                          m_nextSceneID;
};


#endif // !CHARA_SELECT_SCENE_DEFINED


