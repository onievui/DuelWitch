#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class PlayParameterLoader;
class DebugCamera;
class GridFloor;
class Player;
class PlayerManager;
class Element;
class ElementManager;
class IMagic;
class MagicManager;
class EffectManager;
class TargetCamera;
class Field;
class CollisionManager;
class Fade;
enum class SceneID;


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
	void Initialize(ISceneRequest* pSceneRequest) override;
	// プレイシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// プレイシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// プレイシーンを終了する
	void Finalize() override;

private:
	// 当たり判定を行う
	void DetectCollision();

private:
	// リクエストシーンインタフェース
	ISceneRequest*                           m_pSceneRequest;

	// 画面用フェード
	std::unique_ptr<Fade>                    m_fade;
	// 勝敗がついたかどうか
	bool                                     m_isFinished;
	// 遷移先のシーンID
	SceneID                                  m_nextSceneID;

	// パラメータ読み込みクラス
	std::unique_ptr<PlayParameterLoader>     m_parameterLoader;

	// デバッグカメラ
	std::unique_ptr<DebugCamera>             m_debugCamera;
	// ターゲットカメラ
	std::unique_ptr<TargetCamera>            m_targetCamera;
	// グリッド床
	std::unique_ptr<GridFloor>               m_gridFloor;
	// フィールド
	std::unique_ptr<Field>                   m_field;
	// プレイヤーマネージャ
	std::unique_ptr<PlayerManager>           m_playerManager;
	// エレメントマネージャ
	std::unique_ptr<ElementManager>          m_elementManager;
	// 魔法マネージャ
	std::unique_ptr<MagicManager>            m_magicManager;
	// エフェクトマネージャ
	std::unique_ptr<EffectManager>           m_effectManager;

	// 当たり判定処理クラス
	std::unique_ptr<CollisionManager>        m_collisionManager;
	
};


#endif // !PLAY_SCENE_DEFINED
