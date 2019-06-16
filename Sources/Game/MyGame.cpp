#define _CRT_SECURE_NO_WARNINGS

#include "MyGame.h"
#include "DebugCamera.h"
#include "GridFloor.h"
#include "Player.h"
#include "ElementManager.h"
#include "TargetCamera.h"
#include "Field.h"

// コンストラクタ
MyGame::MyGame(int width, int height) : m_width(width), m_height(height), Game(width, height)
{
}

// デストラクタ
MyGame::~MyGame() = default;

// MyGameオブジェクトを初期する
void MyGame::Initialize(int width, int height) 
{
	// 基底クラスのInitializeを呼び出す 
	Game::Initialize(width, height);

	// CommonStatesオブジェクトを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(m_directX.GetDevice().Get());
	// EffectFactoryオブジェクトを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_directX.GetDevice().Get());
	// キーボードを生成する
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	// マウスを生成する
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(DirectX11::Get().GetHWnd());

	m_world = DirectX::SimpleMath::Matrix::Identity;
}

// リソースを生成する
void MyGame::CreateResources()
{
	// 基底クラスのCreateResourcesを呼び出す
	Game::CreateResources();

	// ビュー座標変換行列を生成する
	// 視点, 注視点, 
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f),
		DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Up);
	// 射影座標変換行列を生成する
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f,
		float(m_width) / float(m_height), 0.1f, 500.0f);
	
	// モデルオブジェクトを生成する
	m_model = std::make_unique<Player>();
	m_model->Create(L"bloom.cmo", L"Resources/Models/Protected");
	// エレメントマネージャを作成する
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();

	//デバッグカメラを生成する
	m_debugCamera = std::make_unique<DebugCamera>(m_width, m_height);
	//ターゲットカメラを生成する
	m_targetCamera = std::make_unique<TargetCamera>(m_model.get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, float(m_width) / float(m_height), 0.1f, 10000.0f);
	//グリッド床を生成する
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 200.0f, 50);

	// フィールドを生成する
	m_field = std::make_unique<Field>();

}

// ゲームを更新する
void MyGame::Update(const DX::StepTimer& timer) 
{
	// プレイヤーの更新
	m_model->Update(timer);
	// エレメントマネージャの更新
	m_elementManager->Update(timer);

	if (timer.GetFrameCount() % 30 == 0) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 80);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3::One*-3.0f+area_offset;
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3::One*3.0f + area_offset;
		m_elementManager->CreateElement(area_start, area_end, 1);
	}

	// デバッグカメラの更新
	m_debugCamera->Update();
	// ターゲットカメラの更新
	m_targetCamera->Update();
	// フィールドの更新
	m_field->Update();
}

// ゲームを描画する
void MyGame::Render(const DX::StepTimer& timer) 
{
	// 最初の更新の前は何も描画しないようにする
	if (timer.GetFrameCount() == 0) 
		return;

	// TODO: レンダリングコードを追加する
	float time = float(timer.GetTotalSeconds());

	// Z軸に対して回転させる行列を生成する
	m_world = DirectX::SimpleMath::Matrix::CreateRotationZ(cosf(time) * 1.0f);

	// バッファをクリアする
	Clear();
	// スプライトバッチを開始する
	GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());
	// FPSを描画する
	DrawFPS(timer);
	//ビュー行列を取得する
	//m_view = m_debugCamera->GetCameraMatrix();
	m_view = m_targetCamera->GetViewMatrix();
	m_projection = m_targetCamera->GetProjectionMatrix();
	//グリッド床を描画する
	m_gridFloor->Render(m_view, m_projection);
	// フィールドを描画する
	m_field->Render(m_view, m_projection);
	// モデルを描画する
	m_model->Render(m_view, m_projection);
	// エレメントを描画する
	m_elementManager->Render(m_view, m_projection);

	// スプライトバッチを終了する
	GetSpriteBatch()->End();
	// バックバッファを表示する
	Present();
}

// 後始末をする
void MyGame::Finalize() 
{
	// 基底クラスのFinalizeを呼び出す
	Game::Finalize();

	m_model.reset();
	m_keyboard.reset();
	m_mouse.reset();
	m_debugCamera.reset();
	m_gridFloor.reset();
}

// FPSを描画する
void MyGame::DrawFPS(const DX::StepTimer& timer)
{
	// FPS文字列を生成する
	std::wstring fpsString = L"fps = " + std::to_wstring((unsigned int)timer.GetFramesPerSecond());
	// FPSを描画する
	GetSpriteFont()->DrawString(GetSpriteBatch(), fpsString.c_str(), DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);
}
