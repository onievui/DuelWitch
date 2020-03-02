#pragma once
#ifndef MYGAME_DEFINED
#define MYGAME_DEFINED


#include <Framework\Window.h>
#include <Framework\Game.h>


class SceneManager;
class CommonServices;


/// <summary>
/// ゲームループクラス
/// </summary>
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

public:
	// メッセージ
	void OnSuspending() override;
	void OnResuming() override;

public:
	// FPSを描画する
	void DrawFPS(const DX::StepTimer& timer);

private:
	// 幅
	int                                m_width;
	// 高さ
	int                                m_height;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// ゲームパッド
	std::unique_ptr<DirectX::GamePad>  m_gamePad;

	// コモンサービス
	std::unique_ptr<CommonServices>    m_commonServices;
	// シーンマネージャ
	std::unique_ptr<SceneManager>      m_sceneManager;
	
};


#endif	// MYGAME_DEFINED
