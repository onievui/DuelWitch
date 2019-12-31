#pragma once
#ifndef LOGO_SCENE_DEFINED
#define LOGO_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class Fade;


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

	// 画面用フェード
	std::unique_ptr<Fade>                                    m_fade;
	// フェードの段階
	int                                                      m_fadeStep;
	
};


#endif // !LOGO_SCENE_DEFINED


