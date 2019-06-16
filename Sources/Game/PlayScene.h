#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class DebugCamera;
class GridFloor;
class Player;
class ElementManager;
class TargetCamera;
class Field;

/// <summary>
/// プレイシーンクラス
/// </summary>
class PlayScene : public IScene {
public:
	// プレイシーンを初期化する
	void Initialize(ISceneRequest* sceneRequest) override;
	// プレイシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// プレイシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// プレイシーンを終了する
	void Finalize() override;

private:
	// リクエストシーンインタフェース
	ISceneRequest* m_sceneRequest;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_font;
};


#endif // !PLAY_SCENE_DEFINED


