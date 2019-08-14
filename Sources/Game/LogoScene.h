#pragma once
#ifndef LOGO_SCENE_DEFINED
#define LOGO_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// ロゴシーンクラス
/// </summary>
class LogoScene : public IScene {
public:
	// コンストラクタ
	LogoScene();
	// デストラクタ
	~LogoScene();
public:
	// ロゴシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// ロゴシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// ロゴシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// ロゴシーンを終了する
	void Finalize() override;

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                           m_pSceneRequest;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont>                     m_font;
	// エフェクトファクトリインタフェース
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// コモンステート
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

	// キートラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_tracker;
};


#endif // !LOGO_SCENE_DEFINED


