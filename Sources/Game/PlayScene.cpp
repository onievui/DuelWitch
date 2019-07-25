#include "PlayScene.h"
#include <Framework\DirectX11.h>
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
	DirectX11& directX = DirectX11::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX.GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX.GetDevice().Get());

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
	m_players[0]->Create(L"bloom.cmo", L"Resources/Models/Protected");
	m_players[1]->Create(L"bloom.cmo", L"Resources/Models/Protected");
	m_players[0]->SetOtherPlayer(m_players[1].get());
	m_players[1]->SetOtherPlayer(m_players[0].get());

	//デバッグカメラを生成する
	m_debugCamera = std::make_unique<DebugCamera>(directX.GetWidth(), directX.GetHeight());
	//ターゲットカメラを生成する
	m_targetCamera = std::make_unique<TargetCamera>(m_players[0].get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, float(directX.GetWidth()) / float(directX.GetHeight()), 0.1f, 10000.0f);

	m_players[0]->SetCamera(m_targetCamera.get());
	m_players[1]->SetCamera(m_targetCamera.get());

	//グリッド床を生成する
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 400.0f, 200);
	// フィールドを生成する
	m_field = std::make_unique<Field>();
	// スプライトフォントを生成する
	//m_font = std::make_unique<DirectX::SpriteFont>(DirectX11::Get().GetDevice().Get(), L"myfile.spritefont");
	
}

/// <summary>
/// プレイシーンを更新する
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	// プレイヤーの更新
	for (auto& player : m_players) {
		player->Update(timer);
	}
	// エレメントマネージャの更新
	m_elementManager->Update(timer);
	// 魔法マネージャの更新
	m_magicManager->Update(timer);

	if (timer.GetFrameCount() % 600 == 0) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 23);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3::One*-3;
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3::One*3;
		m_elementManager->CreateElement(area_start + area_offset, area_end + area_offset, 3);
		area_offset.z = 127.0f;
		m_elementManager->CreateElement(area_start + area_offset, area_end + area_offset, 3);
	}

	// 当たり判定
	// プレイヤーとエレメントの当たり判定
	for (auto& element : *m_elementManager->GetElements()) {
		// 未使用なら飛ばす
		if (!element) {
			continue;
		}
		auto* element_collider = element->GetCollider();
		for (auto& player : m_players) {
			if (player->GetCollider()->Collision(element_collider)) {
				player->GetElement(element->GetID());
				element->IsUsed(false);
			}
		}
	}

	// 魔法同士の当たり判定
	{
		auto magics = m_magicManager->GetMagics();
		// 未使用なら飛ばす処理を定義
		auto pred = [](auto& element) {return element; };
		// 魔法同士の当たり判定
		auto itr = std::find_if(magics->begin(), magics->end(), pred);
		for (auto end = magics->end(); itr != end;) {
			auto* collider = (*itr)->GetCollider();
			auto next = std::find_if(itr + 1, end, pred);
			for (auto itr2 = next; itr2 != end;	itr2 = std::find_if(itr2 + 1, end, pred)) {
				// 同一プレイヤーの魔法なら判定しない
				if ((*itr)->GetPlayerID() == (*itr2)->GetPlayerID()) {
					continue;
				}
				if (collider->Collision((*itr2)->GetCollider())) {
					(*itr)->HitMagic(*itr2);
					(*itr2)->HitMagic(*itr);
				}
			}
			itr = next;
		}
	}

	// プレイヤ―と魔法の当たり判定
	for (auto& magic : *m_magicManager->GetMagics()) {
		// 未使用なら飛ばす
		if (!magic) {
			continue;
		}
		auto* magic_collider = magic->GetCollider();
		for (auto& player : m_players) {
			// 自身の魔法とは判定しない
			if (player->GetPlayerID() == magic->GetPlayerID()) {
				continue;
			}
			if (player->GetCollider()->Collision(magic_collider)) {
				magic->HitPlayer(*player->GetCollider());
			}
		}
	}

	// プレイヤー同士の当たり判定
	for (auto& player1 : m_players) {
		auto* player_collider1 = player1->GetCollider();
		for (auto& player2 : m_players) {
			if (player1.get() == player2.get()) {
				continue;
			}
			if (player_collider1->Collision(player2->GetCollider())) {
				player1->CollisionPlayer(*player2);
				player2->CollisionPlayer(*player1);
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
	for (auto& player : m_players) {
		player->Render(view, projection);
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

}

