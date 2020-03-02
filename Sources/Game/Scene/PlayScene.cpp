#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include <Utils\LamdaUtils.h>
#include <Utils\Resource.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Load\ResourceLoader.h>
#include "ISceneRequest.h"
#include <Game\Camera\DebugCamera.h>
#include <Game\Camera\TargetCamera.h>
#include <Game\Player\Player.h>
#include <Game\Player\PlayerID.h>
#include <Game\Player\PlayerManager.h>
#include <Game\Element\Element.h>
#include <Game\Element\ElementManager.h>
#include <Game\Magic\IMagic.h>
#include <Game\Magic\MagicManager.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Field\Field.h>
#include <Game\Field\GridFloor.h>
#include <Game\Collision\CollisionManager.h>
#include <Game\UI\Fade.h>
#include <Parameters\OptionParameter.h>


/// <summary>
/// コンストラクタ
/// </summary>
PlayScene::PlayScene() {
}

/// <summary>
/// デストラクタ
/// </summary>
PlayScene::~PlayScene() {
}

/// <summary>
///	プレイシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void PlayScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();

	// マウスを相対モードに変更する
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
	// マウスを非表示にする
	ServiceLocater<MouseWrapper>::Get()->SetVisible(false);
	// マウスをウインドウ内に収める
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(true);
	// カーソルを画面の中心に移動させる
	ServiceLocater<MouseWrapper>::Get()->SetPos(DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f));

	m_isFinished = false;

	// リソースをロードする
	ResourceLoader::Load(ResourceLoaderID::PlayScene);

	// パラメータをロードする
	m_parameterLoader = std::make_unique<PlayParameterLoader>();
	m_parameterLoader->Load();
	ServiceLocater<PlayParameterLoader>::Register(m_parameterLoader.get());
	
	// マウスの感度を設定する
	ServiceLocater<MouseWrapper>::Get()->SetSensivity(m_parameterLoader->GetOptionParameter()->mouseParam.sensivity);


	// エフェクトマネージャを生成する
	m_effectManager = std::make_unique<EffectManager>();
	m_effectManager->Initialize();
	// エフェクトマネージャをサービスロケータに登録する
	ServiceLocater<EffectManager>::Register(m_effectManager.get());

	// フィールドを生成する
	m_field = std::make_unique<Field>();

	// エレメントマネージャを作成する
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();
	// 魔法マネージャを生成する
	m_magicManager = std::make_unique<MagicManager>();
	m_magicManager->Initialize();
	
	//デバッグカメラを生成する
	m_debugCamera = std::make_unique<DebugCamera>(directX->GetWidth(), directX->GetHeight());
	//ターゲットカメラを生成する
	//DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f)
	m_targetCamera = std::make_unique<TargetCamera>(nullptr, DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
	//m_targetCamera = std::make_unique<TargetCamera>(nullptr, DirectX::SimpleMath::Vector3(0.0f, 2.0f, 5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		PerspectiveFovInfo(Math::HarfPI*0.5f, static_cast<float>(directX->GetWidth()) / static_cast<float>(directX->GetHeight()), 0.1f, 5000.0f));

	ServiceLocater<TargetCamera>::Register(m_targetCamera.get());

	// プレイヤーを生成する
	m_playerManager = std::make_unique<PlayerManager>();
	m_playerManager->Initialize(m_magicManager.get(), m_targetCamera.get());

	// 当たり判定処理クラスを生成する
	m_collisionManager = std::make_unique<CollisionManager>();

	//グリッド床を生成する
	m_gridFloor = std::make_unique<GridFloor>(ServiceLocater<DirectX::CommonStates>::Get(), 200.0f, 100);
	
	// フェードを生成する
	m_fade = std::make_unique<Fade>();
	m_fade->Initialize(Fade::State::FadeIn, 1.0f, 0.0f);

	// BGMを再生する
	ServiceLocater<AudioManager>::Get()->PlayBgm(BgmID::Battle);
	// BGMをフェードインさせる
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, 0.0f, 1.0f);
}

/// <summary>
/// プレイシーンを更新する
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {

	// フェードを更新する
	m_fade->Update(timer);

	// エスケープキーでポーズ画面を呼び出す
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		m_pSceneRequest->RequestScene(SceneID::Pause, RequestSceneType::StackScene);
	}

	// F2キーでパラメータを再読み込みする
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F2)) {
		m_parameterLoader->Reload();
		// マウスの感度を設定する
		ServiceLocater<MouseWrapper>::Get()->SetSensivity(m_parameterLoader->GetOptionParameter()->mouseParam.sensivity);
	}

	// 勝敗を判定する
	if (!m_isFinished) {
		if (m_playerManager->Player1Win() || ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F3)) {
			m_nextSceneID = SceneID::WinResult;
			// 勝利した場合は白くフェードアウトする
			m_fade->Initialize(Fade::State::FadeOut, 3.0f, 0.7f, static_cast<DirectX::SimpleMath::Color>(DirectX::Colors::White));
			m_isFinished = true;
		}
		else if (m_playerManager->Player1Lose() || ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F4)) {
			m_nextSceneID = SceneID::LoseResult;
			// 敗北した場合は黒くフェードアウトする
			m_fade->Initialize(Fade::State::FadeOut, 3.0f, 0.8f, static_cast<DirectX::SimpleMath::Color>(DirectX::Colors::Black));
			m_isFinished = true;
		}
		if (m_isFinished) {
			// BGMをフェードアウトさせる
			ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, 1.0f, 0.0f);
		}
	}
	else if(m_fade->IsFinished()) {
		// フェードアウトが完了したらシーン遷移する
		m_pSceneRequest->RequestScene(m_nextSceneID, RequestSceneType::StackScene);
		// マウスを表示する
		ServiceLocater<MouseWrapper>::Get()->SetVisible(true);
		// マウスをウインドウ内に移動できるようにする
		ServiceLocater<MouseWrapper>::Get()->ClipToWindow(false);
	}

	// プレイヤーマネージャを更新する
	m_playerManager->Update(timer);

	// エレメントマネージャを更新する
	m_elementManager->SetRadius(m_field->GetRadius());
	m_elementManager->Update(timer);
	// 魔法マネージャを更新する
	m_magicManager->Update(timer);
	// エフェクトマネージャを更新する
	m_effectManager->Update(timer, m_targetCamera.get());

	// 当たり判定を行う
	DetectCollision();

	// フィールドの更新
	m_field->Update(timer);

	// デバッグカメラの更新
	m_debugCamera->Update();
	// ターゲットカメラの更新
	m_targetCamera->Update();
	
}

/// <summary>
/// プレイシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void PlayScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	//ビュー行列を取得する
	DirectX::SimpleMath::Matrix view = m_targetCamera->GetViewMatrix();
	// 射影行列を取得する
	DirectX::SimpleMath::Matrix projection = m_targetCamera->GetProjectionMatrix();

	//グリッド床を描画する
	//m_gridFloor->Render(view, projection);
	// フィールドを描画する
	m_field->Render(view, projection);

	// エレメントを描画する
	m_elementManager->Render(view, projection);
	// 魔法を描画する
	m_magicManager->Render(view, projection);
	
	// プレイヤーを描画する
	m_playerManager->Render(view, projection, spriteBatch);

	// エフェクトを描画する
	m_effectManager->Render(view, projection);

	// シーンの最初と終わりでフェードイン・フェードアウトする
	m_fade->Render(spriteBatch);

	spriteBatch->End();

	// プレイヤー1の画面エフェクトを描画する
	m_playerManager->RenderPlayer1Effect(spriteBatch);

}

/// <summary>
/// プレイシーンを終了する
/// </summary>
void PlayScene::Finalize() {
	//// マウスを絶対モードに変更する
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
	// マウスを表示する
	ServiceLocater<MouseWrapper>::Get()->SetVisible(true);
	// マウスをウインドウ内に移動できるようにする
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(false);
	// リソースを解放する
	ResourceLoader::Release(ResourceLoaderID::PlayScene);
	// パラメータを開放する
	m_parameterLoader->Dispose();
	ServiceLocater<PlayParameterLoader>::Unregister();
	// エフェクトマネージャをサービスロケータから解除する
	ServiceLocater<EffectManager>::Unregister();
}

/// <summary>
/// 当たり判定を行う
/// </summary>
void PlayScene::DetectCollision() {
	// プレイヤーとフィールドの当たり判定を行う
	m_collisionManager->CollisionPlayerField(m_playerManager->GetPlayers(), m_field.get());
	
	// プレイヤーとエレメントの当たり判定を行う
	m_collisionManager->CollisionPlayerElement(m_playerManager->GetPlayers(), m_elementManager->GetElements());

	// 魔法同士の当たり判定を行う
	m_collisionManager->CollisionMagic(m_magicManager->GetMagics());

	// プレイヤーと魔法の当たり判定を行う
	m_collisionManager->CollisionPlayerMagic(m_playerManager->GetPlayers(), m_magicManager->GetMagics());

	// プレイヤー同士の当たり判定を行う
	m_collisionManager->CollisionPlayer(m_playerManager->GetPlayers());
	
}

