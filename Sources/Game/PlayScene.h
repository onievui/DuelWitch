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
	// コンストラクタ
	PlayScene();
	// デストラクタ
	~PlayScene();
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
	// エフェクトファクトリインタフェース(m_fxFactory)
	std::unique_ptr<DirectX::IEffectFactory> m_effectFactory;
	// コモンステート
	std::unique_ptr <DirectX::CommonStates> m_commonStates;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// マウス
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugCamera;
	// ターゲットカメラ
	std::unique_ptr<TargetCamera> m_targetCamera;
	// グリッド床
	std::unique_ptr<GridFloor> m_gridFloor;

	// フィールド
	std::unique_ptr<Field> m_field;

	// モデル
	std::unique_ptr<Player> m_model;
	// エレメントマネージャ
	std::unique_ptr<ElementManager> m_elementManager;
};


#endif // !PLAY_SCENE_DEFINED


