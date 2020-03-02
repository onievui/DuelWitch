#pragma once
#ifndef CHARA_MANUAL_SCENE_DEFINED
#define CHARA_MANUAL_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class SoundScaleUpUI;
class Fade;
enum class SceneID;

/// <summary>
/// キャラマニュアルシーンクラス
/// </summary>
class CharaManualScene : public IScene {
public:
	// キャラマニュアルの表示にかかる時間
	static constexpr float CHARA_MANUAL_TIME = 0.2f;

public:
	// コンストラクタ
	CharaManualScene();
	// デストラクタ
	~CharaManualScene();

public:
	// キャラマニュアルシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// キャラマニュアルシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// キャラマニュアルシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// キャラマニュアルシーンを終了する
	void Finalize() override;

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                     m_pSceneRequest;

	// 画面用フェード
	std::unique_ptr<Fade>                              m_fade;
	// テクスチャ用フェード
	std::unique_ptr<Fade>                              m_fadeTexture;
	// マニュアルの表示を解除したかどうか
	bool                                               m_canceled;
};


#endif // !CHARA_MANUAL_SCENE_DEFINED


