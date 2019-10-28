#pragma once
#ifndef PAUSE_SCENE_DEFINED
#define PAUSE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;


/// <summary>
/// ポーズシーンクラス
/// </summary>
class PauseScene : public IScene {
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
	// ポーズを解除する
	void Resume();

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                           m_pSceneRequest;
	// エフェクトファクトリインタフェース
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// コモンステート
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

	// UIオブザーバ
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// メニューUI
	std::vector<std::unique_ptr<ScaleUpUI>>                  m_menuUIs;

};


#endif // !PAUSE_SCENE_DEFINED


