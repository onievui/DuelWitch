#include "Game.h"
#include <Utils\ServiceLocater.h>


void ExitGame();

// コンストラクタ
Game::Game(int width, int height)
	: m_hWnd(0)
	, m_width(width)
	, m_height(height)
	, m_featureLevel(D3D_FEATURE_LEVEL_9_1) {
	// スタートアップ情報
	STARTUPINFO si{};
	// インスタンスハンドルを取得する
	m_hInstance = ::GetModuleHandle(NULL);

	// STARTUPINFO構造体を取得する
	::GetStartupInfo(&si);
	m_nCmdShow = si.dwFlags & STARTF_USESHOWWINDOW ? si.wShowWindow : SW_SHOWDEFAULT;
	// Windowオブジェクトを生成する
	m_window = std::make_unique<Window>(m_hInstance, m_nCmdShow);

	m_directX = std::make_unique<DirectX11>();
	ServiceLocater<DirectX11>::Register(m_directX.get());
}

// ゲームリソースを初期化する
void Game::Initialize(int width, int height) {
	width, height;
	// Windowオブジェクトを初期化する
	m_window->Initialize(m_width, m_height);
	// Windowオブジェクトの生成後にウィンドウハンドルを取得する
	m_hWnd = m_window->GetHWnd();

	// DirectXの初期化のためウィンドウハンドルを設定する
	m_directX->SetHWnd(m_hWnd);
	// DirectXの初期化のためウィンドウ幅を設定する
	m_directX->SetWidth(m_width);
	// DirectXの初期化のためウィンドウ高を設定する
	m_directX->SetHeight(m_height);

	// デバイスを生成する
	m_directX->CreateDevice();
	// リソースを生成する
	m_directX->CreateResources();

    // TODO: デフォルト変数timestepモード以外のものが必要な場合タイマー設定を変更する
	// 例: 60FPS固定タイムステップ更新ロジックに対しては以下を呼び出す
    
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60.0);

	// SpriteBatchオブジェクトを生成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_directX->GetContext().Get());
	// 背景色を設定する
	m_backGroundColor = DirectX::SimpleMath::Color(DirectX::Colors::CornflowerBlue);
}

// リソースを生成する
void Game::CreateResources() {
}

// ゲームループを実行する
MSG Game::Run() {
	// メッセージ
	MSG msg = {};

	// Gameオブジェクトを初期化する
	Initialize(m_width, m_height);
	// ウィンドウを表示する
	m_window->ShowWindow();
	// リソースを生成する
	CreateResources();

	// ゲームループ
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// ゲームを更新する
			m_timer.Tick([&]() { Update(m_timer); });
			// ゲームシーンを描画する
			Render(m_timer);
		}
	}
	// Gameオブジェクトの後始末をする
	Finalize();
	return msg;
}

// ゲームを更新する
void Game::Update(const DX::StepTimer& timer) {
	timer;
}

// 後始末をする
void Game::Finalize() {
	// SpriteBatchオブジェクトを解放する
	m_spriteBatch.reset();

	// DirectX11 Graphicsオブジェクトを解放する
	m_directX->OnDeviceLost();
	ServiceLocater<DirectX11>::Unregister();
	// Windowオブジェクトを解放する
	m_window.reset();
}

// シーンを描画する
void Game::Render(const DX::StepTimer& timer) {
	timer;
}

// バックバッファをクリアする
void Game::Clear() {
	ID3D11DeviceContext* context = m_directX->GetContext().Get();
	ID3D11RenderTargetView* render_target_view = m_directX->GetRenderTargetView().Get();
	ID3D11DepthStencilView* depth_stencil_view = m_directX->GetDepthStencilView().Get();

	// レンダーターゲットビューをクリアする
	context->ClearRenderTargetView(render_target_view, m_backGroundColor);
	// デプスステンシルビューをクリアする
	context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// レンダータッゲートを設定する
	context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
    // ビューポートを設定する
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height));
	context->RSSetViewports(1, &viewport);
}

// バックバッファをスクリーンに送る 
void Game::Present() {
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.

	HRESULT hr = m_directX->GetSwapChain()->Present(1, 0);

    // デバイスがリセットされた場合レンダラを再初期化する必要がある 
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) 
		m_directX->OnDeviceLost();
    else 
        DX::ThrowIfFailed(hr);
}

// メッセージハンドラ
void Game::OnActivated() {
	// TODO: ゲームがアクティブなウィンドウになる場合
}

void Game::OnDeactivated() {
    // TODO: ゲームがバックグラウンドウィンドウになる場合 
}

void Game::OnSuspending() {
    // TODO: ゲームがパワーサスペンデッドになる場合
}

void Game::OnResuming() {
	m_timer.ResetElapsedTime();

    // TODO: ゲームがパワーレジュームになる場合
}

void Game::OnWindowSizeChanged(int width, int height) {
    m_width = std::max(width, 1);
    m_height = std::max(height, 1);
	
	m_directX->CreateResources();
    // TODO: ゲームウィンドウのサイズが再変更された場合
}

// プロパティ Properties
void Game::GetDefaultSize(int& width, int& height) const {
    // TODO: 任意のデフォルトウィンドウサイズに変更する(最小サイズは320x200) 
    width = 800;
    height = 600;
}

// Exitヘルパー関数
void ExitGame() {
	PostQuitMessage(0);
}
