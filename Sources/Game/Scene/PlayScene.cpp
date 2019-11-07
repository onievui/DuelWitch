#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include <Utils\LamdaUtils.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Load\ResourceLoader.h>
#include "ISceneRequest.h"
#include <Game\Camera\DebugCamera.h>
#include <Game\Camera\TargetCamera.h>
#include <Game\Player\Player.h>
#include <Game\Player\PlayerID.h>
#include <Game\Element\Element.h>
#include <Game\Element\ElementManager.h>
#include <Game\Magic\IMagic.h>
#include <Game\Magic\MagicManager.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Field\Field.h>
#include <Game\Field\GridFloor.h>
#include <Game\Collision\Collision.h>


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
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	// マウスを相対モードに変更する
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
	// カーソルを画面の中心に移動させる
	ServiceLocater<MouseWrapper>::Get()->SetPos(DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f));

	// リソースをロードする
	ResourceLoader::Load(ResourceLoaderID::PlayScene);

	// パラメータをロードする
	m_parameterLoader = std::make_unique<PlayParameterLoader>();
	m_parameterLoader->Load();
	ServiceLocater<PlayParameterLoader>::Register(m_parameterLoader.get());

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
	m_pMagics = m_magicManager->GetMagics();
	
	// プレイヤーを生成する
	DirectX::SimpleMath::Vector3 player_pos(0, 0, -75);
	DirectX::SimpleMath::Quaternion player_pos_rot =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, Math::PI2 / 3);
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player1, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player2, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player3, player_pos));

	//デバッグカメラを生成する
	m_debugCamera = std::make_unique<DebugCamera>(directX->GetWidth(), directX->GetHeight());
	//ターゲットカメラを生成する
	m_targetCamera = std::make_unique<TargetCamera>(nullptr, DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		PerspectiveFovInfo(Math::HarfPI*0.5f, static_cast<float>(directX->GetWidth()) / static_cast<float>(directX->GetHeight()), 0.1f, 5000.0f));

	// プレイヤーを初期化する
	m_players[0]->Initialize(m_magicManager.get(), m_targetCamera.get(), m_players);
	m_players[1]->Initialize(m_magicManager.get(), m_targetCamera.get(), m_players);
	m_players[2]->Initialize(m_magicManager.get(), m_targetCamera.get(), m_players);

	//グリッド床を生成する
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 200.0f, 100);
	
}

/// <summary>
/// プレイシーンを更新する
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	// エスケープキーでポーズ画面を呼び出す
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		m_pSceneRequest->RequestScene("Pause", RequestSceneType::StackScene);
	}

	// F2キーでパラメータを再読み込みする
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F2)) {
		m_parameterLoader->Reload();
	}

	// プレイヤーを更新する
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// エレメントマネージャを更新する
	m_elementManager->SetRadius(m_field->GetRadius());
	m_elementManager->Update(timer);
	// 魔法マネージャを更新する
	m_magicManager->Update(timer);
	// エフェクトマネージャを更新する
	m_effectManager->Update(timer, m_targetCamera.get());

	// プレイヤーとフィールドの当たり判定
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		m_field->CollisionCheckPlayer(*(*itr));
	}

	// 未使用なら飛ばす処理を生成
	auto live_pred = LamdaUtils::NotNull();
	
	// 当たり判定
	// プレイヤーとエレメントの当たり判定
	for (std::vector<Element*>::iterator element_itr = LamdaUtils::FindIf(*m_elementManager->GetElements(), live_pred),
		element_end = m_elementManager->GetElements()->end();
		element_itr != element_end;
		LamdaUtils::FindIfNext(element_itr, element_end, live_pred)) {
		const Collider* element_collider = (*element_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			if (Collision::HitCheck(element_collider, (*player_itr)->GetCollider())) {
				(*player_itr)->GetElement((*element_itr)->GetID());
				(*element_itr)->SetUsed(false);
			}
		}
	}
	

	// 魔法同士の当たり判定
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_magicManager->GetMagics(), live_pred),
		end = m_magicManager->GetMagics()->end();
		itr != end;) {
		const Collider* collider = (*itr)->GetCollider();
		std::vector<IMagic*>::iterator next = std::find_if(itr + 1, end, live_pred);
		for (std::vector<IMagic*>::iterator itr2 = next; itr2 != end; itr2 = std::find_if(itr2 + 1, end, live_pred)) {
			// 同一プレイヤーの魔法なら判定しない
			if ((*itr)->GetPlayerID() == (*itr2)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(collider, (*itr2)->GetCollider())) {
				(*itr)->HitMagic(*itr2);
				(*itr2)->HitMagic(*itr);
			}
		}
		itr = next;
	}

	// プレイヤ―と魔法の当たり判定
	for (std::vector<IMagic*>::iterator magic_itr = LamdaUtils::FindIf(*m_magicManager->GetMagics(), live_pred),
		magic_end = m_magicManager->GetMagics()->end();
		magic_itr != magic_end;
		LamdaUtils::FindIfNext(magic_itr, magic_end, live_pred)) {
		const Collider* magic_collider = (*magic_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			// 自身の魔法とは判定しない
			if ((*player_itr)->GetPlayerID() == (*magic_itr)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(magic_collider, (*player_itr)->GetCollider())) {
				(*magic_itr)->HitPlayer((*player_itr)->GetCollider());
				(*player_itr)->HitMagic(*magic_itr);
			}
		}
	}

	// プレイヤー同士の当たり判定
	for (std::vector<std::unique_ptr<Player>>::iterator itr1 = m_players.begin(); itr1 != m_players.end() - 1; ++itr1) {
		const Collider* collider1 = (*itr1)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator itr2 = itr1 + 1; itr2 != m_players.end(); ++itr2) {
			if (Collision::HitCheck(collider1, (*itr2)->GetCollider())) {
				(*itr1)->HitPlayer(**itr2);
				(*itr2)->HitPlayer(**itr1);
			}
		}
	}

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
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

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
	for (std::vector<std::unique_ptr<Player>>::const_iterator itr = m_players.cbegin(); itr != m_players.cend(); ++itr) {
		(*itr)->Render(view, projection, spriteBatch);
	}

	// エフェクトを描画する
	m_effectManager->Render(view, projection);

	spriteBatch->End();
}

/// <summary>
/// プレイシーンを終了する
/// </summary>
void PlayScene::Finalize() {
	// マウスを絶対モードに変更する
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
	// リソースを解放する
	ResourceLoader::Release(ResourceLoaderID::PlayScene);
	// パラメータを開放する
	m_parameterLoader->Dispose();
	ServiceLocater<PlayParameterLoader>::Unregister();
	// エフェクトマネージャをサービスロケータから解除する
	ServiceLocater<EffectManager>::Unregister();
}

