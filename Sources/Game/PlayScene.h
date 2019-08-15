#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class DebugCamera;
class GridFloor;
class Player;
class Element;
class ElementManager;
class IMagic;
class MagicManager;
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
	void Initialize(ISceneRequest* pSceneRequest) override;
	// プレイシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// プレイシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// プレイシーンを終了する
	void Finalize() override;

private:
	// リクエストシーンインタフェース
	ISceneRequest*                           m_pSceneRequest;
	// エフェクトファクトリインタフェース
	std::unique_ptr<DirectX::IEffectFactory> m_effectFactory;
	// コモンステート
	std::unique_ptr <DirectX::CommonStates>  m_commonStates;

	// デバッグカメラ
	std::unique_ptr<DebugCamera>             m_debugCamera;
	// ターゲットカメラ
	std::unique_ptr<TargetCamera>            m_targetCamera;
	// グリッド床
	std::unique_ptr<GridFloor>               m_gridFloor;
	// フィールド
	std::unique_ptr<Field>                   m_field;
	// プレイヤー
	std::vector<std::unique_ptr<Player>>     m_players;
	// エレメントマネージャ
	std::unique_ptr<ElementManager>          m_elementManager;
	// 魔法マネージャ
	std::unique_ptr<MagicManager>            m_magicManager;
	// 魔法
	std::vector<IMagic*>*                    m_pMagics;
	
};


#endif // !PLAY_SCENE_DEFINED


