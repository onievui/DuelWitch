#pragma once
#ifndef MYGAME_DEFINED
#define MYGAME_DEFINED


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

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// マウス
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// シーンマネージャ
	std::unique_ptr<SceneManager> m_sceneManager;

};

#endif	// MYGAME_DEFINED