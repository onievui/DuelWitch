#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;


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
	// エフェクトファクトリインタフェース
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// コモンステート
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

	// タイマー
	float                                                    m_time;
	// UIオブザーバ
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// メニューUI
	std::vector<std::unique_ptr<ScaleUpUI>>                  m_menuUIs;

};


#endif // !TITLE_SCENE_DEFINED


