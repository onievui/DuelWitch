#pragma once
#ifndef MYGAME_DEFINED
#define MYGAME_DEFINED

#include <Framework\DirectX11.h>
#include <Framework\Window.h>
#include <Framework\Game.h>


class SceneManager;


class MyGame : public Game {
public:
	// コンストラクタ
	MyGame(int width, int height);
	// デストラクタ
	~MyGame();
	// ゲームを初期化する
	void Initialize(int width, int height) override;
	// リソースを生成する
	void CreateResources() override;
	// ゲームを更新する
	void Update(const DX::StepTimer& timer) override;
	// ゲームを描画する
	void Render(const DX::StepTimer& timer) override;
	// 終了処理をおこなう
	void Finalize() override;

	// FPSを描画する
	void DrawFPS(const DX::StepTimer& timer);

private:
	// 幅
	int m_width;
	// 高さ
	int m_height;
	//// ワールド行列
	//DirectX::SimpleMath::Matrix m_world;
	//// ビュー行列
	//DirectX::SimpleMath::Matrix m_view;
	//// 射影行列
	//DirectX::SimpleMath::Matrix m_projection;
	//
	//// エフェクトファクトリインタフェース(m_fxFactory)
	//std::unique_ptr<DirectX::IEffectFactory> m_effectFactory;
	//// コモンステート
	//std::unique_ptr <DirectX::CommonStates> m_commonStates;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// マウス
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// シーンマネージャ
	std::unique_ptr<SceneManager> m_sceneManager;

	//// デバッグカメラ
	//std::unique_ptr<DebugCamera> m_debugCamera;
	//// ターゲットカメラ
	//std::unique_ptr<TargetCamera> m_targetCamera;
	//// グリッド床
	//std::unique_ptr<GridFloor> m_gridFloor;
	//
	//// フィールド
	//std::unique_ptr<Field> m_field;
	//
	//// モデル
	//std::unique_ptr<Player> m_model;
	//// エレメントマネージャ
	//std::unique_ptr<ElementManager> m_elementManager;

	// DirectX11クラスのインスタンスを取得する
	DirectX11& m_directX = DirectX11::Get();
};

#endif	// MYGAME_DEFINED