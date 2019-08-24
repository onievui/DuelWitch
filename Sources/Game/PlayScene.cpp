#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\LoadDataManager.h>
#include "ResourceLoader.h"
#include "ISceneRequest.h"
#include "DebugCamera.h"
#include "GridFloor.h"
#include "Player.h"
#include "Element.h"
#include "ElementManager.h"
#include "IMagic.h"
#include "MagicManager.h"
#include "TargetCamera.h"
#include "Field.h"
#include "Collision.h"


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

	// リソースをロードする
	ResourceLoader::Load(ResourceLoaderID::PlayScene);

	// パラメータをロードする
	LoadDataManager::GetIns()->Load(LoadDataID::PlayScene);

	// エレメントマネージャを作成する
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();
	// 魔法マネージャを生成する
	m_magicManager = std::make_unique<MagicManager>();
	m_magicManager->Initialize();
	m_pMagics = m_magicManager->GetMagics();

	// プレイヤーを生成する
	m_players.emplace_back(std::make_unique<Player>(m_magicManager.get(), PlayerID::Player1,
		DirectX::SimpleMath::Vector3::Zero, Player::MoveDirection::Forward));
	m_players.emplace_back(std::make_unique<Player>(m_magicManager.get(), PlayerID::Player2,
		DirectX::SimpleMath::Vector3(0, 0, 150), Player::MoveDirection::Backward));
	m_players[0]->Create();
	m_players[1]->Create();
	m_players[0]->SetOtherPlayer(m_players[1].get());
	m_players[1]->SetOtherPlayer(m_players[0].get());

	//デバッグカメラを生成する
	m_debugCamera = std::make_unique<DebugCamera>(directX->GetWidth(), directX->GetHeight());
	//ターゲットカメラを生成する
	m_targetCamera = std::make_unique<TargetCamera>(m_players[0].get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, static_cast<float>(directX->GetWidth()) / static_cast<float>(directX->GetHeight()), 0.1f, 10000.0f);

	m_players[0]->SetCamera(m_targetCamera.get());
	m_players[1]->SetCamera(m_targetCamera.get());

	//グリッド床を生成する
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 400.0f, 200);
	// フィールドを生成する
	m_field = std::make_unique<Field>();
	
}

/// <summary>
/// プレイシーンを更新する
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	// プレイヤーの更新
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// エレメントマネージャの更新
	m_elementManager->Update(timer);
	// 魔法マネージャの更新
	m_magicManager->Update(timer);

	static float time = 0;
	time += static_cast<float>(timer.GetElapsedSeconds());
	if (time >= 10.0f) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 23);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3(-3, -3, -2);
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3(3, 3, 2);
		m_elementManager->CreateElement(area_start + area_offset, area_end + area_offset, 3);
		area_offset.z = 127.0f;
		m_elementManager->CreateElement(area_start + area_offset, area_end + area_offset, 3);
		time -= 10.0f;
	}

	// 当たり判定
	// プレイヤーとエレメントの当たり判定
	for (std::vector<Element*>::iterator element_itr = m_elementManager->GetElements()->begin(); 
		element_itr != m_elementManager->GetElements()->end(); ++element_itr) {
		// 未使用なら飛ばす
		if (!*element_itr) {
			continue;
		}
		const SphereCollider* element_collider = (*element_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			if (Collision::HitCheck(element_collider,(*player_itr)->GetCollider())) {
				(*player_itr)->GetElement((*element_itr)->GetID());
				(*element_itr)->IsUsed(false);
			}
		}
	}

	// 魔法同士の当たり判定
	{
		// 未使用なら飛ばす処理を定義
		auto pred = [](IMagic* magic) {return magic; };
		// 魔法同士の当たり判定
		std::vector<IMagic*>::iterator itr = std::find_if(m_magicManager->GetMagics()->begin(), m_magicManager->GetMagics()->end(), pred);
		for (std::vector<IMagic*>::iterator end = m_magicManager->GetMagics()->end(); itr != end;) {
			const SphereCollider* collider = (*itr)->GetCollider();
			std::vector<IMagic*>::iterator next = std::find_if(itr + 1, end, pred);
			for (std::vector<IMagic*>::iterator itr2 = next; itr2 != end; itr2 = std::find_if(itr2 + 1, end, pred)) {
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
	}

	// プレイヤ―と魔法の当たり判定
	for (std::vector<IMagic*>::iterator magic_itr = (*m_magicManager->GetMagics()).begin();
		magic_itr != (*m_magicManager->GetMagics()).end(); ++magic_itr) {
		// 未使用なら飛ばす
		if (!*magic_itr) {
			continue;
		}
		const SphereCollider* magic_collider = (*magic_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			// 自身の魔法とは判定しない
			if ((*player_itr)->GetPlayerID() == (*magic_itr)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(magic_collider,(*player_itr)->GetCollider())) {
				(*magic_itr)->HitPlayer(*(*player_itr)->GetCollider());
				(*player_itr)->HitMagic(*magic_itr);
			}
		}
	}

	// プレイヤー同士の当たり判定
	for (std::vector<std::unique_ptr<Player>>::iterator itr1 = m_players.begin(); itr1 != m_players.end() - 1; ++itr1) {
		const SphereCollider* collider1 = (*itr1)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator itr2 = itr1 + 1; itr2 != m_players.end(); ++itr2) {
			if (Collision::HitCheck(collider1, (*itr2)->GetCollider())) {
				(*itr1)->HitPlayer(**itr2);
				(*itr2)->HitPlayer(**itr1);
			}
		}
	}
	



	// フィールドの更新
	m_field->Update();

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
	spriteBatch->Begin();

	//ビュー行列を取得する
	DirectX::SimpleMath::Matrix view = m_targetCamera->GetViewMatrix();
	// 射影行列を取得する
	DirectX::SimpleMath::Matrix projection = m_targetCamera->GetProjectionMatrix();

	//グリッド床を描画する
	m_gridFloor->Render(view, projection);
	// フィールドを描画する
	m_field->Render(view, projection);
	// プレイヤーを描画する
	for (std::vector<std::unique_ptr<Player>>::const_iterator itr = m_players.cbegin(); itr != m_players.cend(); ++itr) {
		(*itr)->Render(view, projection, spriteBatch);
	}
	// エレメントを描画する
	m_elementManager->Render(view, projection);
	// 魔法を描画する
	m_magicManager->Render(view, projection);
	
	spriteBatch->End();
}

/// <summary>
/// プレイシーンを終了する
/// </summary>
void PlayScene::Finalize() {
	// リソースを解放する
	ResourceLoader::Release();
	// パラメータを開放する
	LoadDataManager::GetIns()->Dispose(LoadDataID::PlayScene);
}

