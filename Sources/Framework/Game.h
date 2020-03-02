#pragma once
#ifndef GAME_DEFINED
#define GAME_DEFINED


#include "StepTimer.h"
#include "Window.h"
#include "DirectX11.h"


class Window;


// ゲームループを提供するゲームクラス
class Game {
public:
	// コンストラクタ
	Game(int width, int height);

public:
	// ゲームループを実行する
	MSG Run();

    // 初期化をおこなう
    virtual void Initialize(int width, int height);
	// リソースを生成する
	virtual void CreateResources();
	// 更新する
	virtual void Update(const DX::StepTimer& timer);
	// 画面をクリアする
	virtual void Clear();
	// シーンを描画する
	virtual void Render(const DX::StepTimer& timer);
	// バックバッファをスクリーンに送る
    virtual void Present();
	// 後始末をする
	virtual void Finalize();

    // メッセージ
    virtual void OnActivated();
    virtual void OnDeactivated();
    virtual void OnSuspending();
    virtual void OnResuming();
    virtual void OnWindowSizeChanged(int width, int height);

    // プロパティ 
    void GetDefaultSize(int& width, int& height) const;

public:
	// スプライトバッチを取得する
	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	// 背景色を設定する
	void SetBackGroundColor(const DirectX::SimpleMath::Color& color) { m_backGroundColor = color; }
	// 背景色を取得する
	const DirectX::SimpleMath::Color& GetBackGroundColor() const { return m_backGroundColor; }

private:
	// 出力幅
	int m_width;
	// 出力高
	int m_height;
	// ウィンドウハンドル
	HWND m_hWnd;
	// ウィンドウ表示フラグ
	int m_nCmdShow;
	// インスタンスハンドル
	HINSTANCE m_hInstance;

	// タイマー
    DX::StepTimer m_timer;
	// 機能レベル
    D3D_FEATURE_LEVEL m_featureLevel;
	// ウィンドウ
	std::unique_ptr<Window> m_window;
	
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// DirectX11クラスのインスタンス
	std::unique_ptr<DirectX11>            m_directX;
	// 背景色
	DirectX::SimpleMath::Color            m_backGroundColor;
};

#endif	// GAME_DEFINED

