#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;
class Fade;
enum class SceneID;

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene : public IScene {
public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene();

public:
	// タイトルシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// タイトルシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// タイトルシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// タイトルシーンを終了する
	void Finalize() override;

private:
	// UIを初期化する
	void InitializeUI();

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                           m_pSceneRequest;

	// 画面用フェード
	std::unique_ptr<Fade>                                    m_fadeScreen;
	// 画面用フェードの段階
	int                                                      m_fadeScreenStep;
	// UI用フェード
	std::unique_ptr<Fade>                                    m_fadeUI;
	// UI用フェードの段階
	int                                                      m_fadeUIStep;

	// UIオブザーバ
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// メニューUI
	std::vector<std::unique_ptr<ScaleUpUI>>                  m_menuUIs;

	// 選択済みかどうか
	bool                                                     m_wasSelected;
	// 遷移先のシーンID
	SceneID                                                  m_nextSceneID;
};


#endif // !TITLE_SCENE_DEFINED


