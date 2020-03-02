#pragma once
#ifndef LOSE_RESULT_SCENE_DEFINED
#define LOSE_RESULT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class MenuUI;
class Fade;
enum class SceneID;


/// <summary>
/// 敗北リザルトシーンクラス
/// </summary>
class LoseResultScene : public IScene {
public:
	// コンストラクタ
	LoseResultScene();
	// デストラクタ
	~LoseResultScene();

public:
	// 敗北リザルトシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// 敗北リザルトシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// 敗北リザルトシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// 敗北リザルトシーンを終了する
	void Finalize() override;

private:
	// UIを初期化する
	void InitializeUI();

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                           m_pSceneRequest;

	// 画面用フェード
	std::unique_ptr<Fade>                                    m_fadeScreen;
	// リザルトロゴ用フェード
	std::unique_ptr<Fade>                                    m_fadeLogo;
	// UI用フェード
	std::unique_ptr<Fade>                                    m_fadeUI;

	// UIオブザーバ
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// メニューUI
	std::vector<std::unique_ptr<MenuUI>>             m_menuUIs;


	// 選択されたかどうか
	bool                                                     m_wasSelected;
	// 遷移先のシーンID
	SceneID                                                  m_nextSceneID;

};


#endif // !LOSE_RESULT_SCENE_DEFINED
