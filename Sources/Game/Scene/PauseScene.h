#pragma once
#ifndef PAUSE_SCENE_DEFINED
#define PAUSE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class MenuUI;
class Fade;
enum class SceneID;

/// <summary>
/// ポーズシーンクラス
/// </summary>
class PauseScene : public IScene {
public:
	// ポーズにかかる時間
	static constexpr float PAUSE_TIME = 0.2f;
	// ポーズ中のBGM音量
	static constexpr float BGM_VOLUME = 0.3f;

public:
	// コンストラクタ
	PauseScene();
	// デストラクタ
	~PauseScene();

public:
	// ポーズシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// ポーズシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// ポーズシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// ポーズシーンを終了する
	void Finalize() override;

private:
	// UIを初期化する
	void InitializeUI();
	// ポーズの解除を選択する
	void SelectResume();
	// ポーズを解除する
	void Resume();

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                     m_pSceneRequest;

	// 奥側画面用フェード
	std::unique_ptr<Fade>                              m_fadeBack;
	// 手前画面用フェード
	std::unique_ptr<Fade>                              m_fadeFront;
	// UI用フェード
	std::unique_ptr<Fade>                              m_fadeUI;

	// UIオブザーバ
	std::unique_ptr<UIObserver>                        m_uiObserver;
	// メニューUI
	std::vector<std::unique_ptr<MenuUI>>               m_menuUIs;

	// 選択中のUI
	int                                                m_selectedUI;
	// UIのフェードが完了したかどうか
	bool                                                     m_fadeUIFinished;
	// 選択済みかどうか
	bool                                               m_wasSelected;
	// 遷移先のシーンID
	SceneID                                            m_nextSceneID;
};


#endif // !PAUSE_SCENE_DEFINED


