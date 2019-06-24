#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include "ISceneRequest.h"
#include "DebugCamera.h"
#include "GridFloor.h"
#include "Player.h"
#include "Element.h"
#include "ElementManager.h"
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

	// モデルオブジェクトを生成する
	m_player = std::make_unique<Player>();
	m_player->Create(L"bloom.cmo", L"Resources/Models/Protected");
	// エレメントマネージャを作成する
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();
	m_pElements = m_elementManager->GetElements();
	//デバッグカメラを生成する
	m_debugCamera = std::make_unique<DebugCamera>(directX.GetWidth(), directX.GetHeight());
	//ターゲットカメラを生成する
	m_targetCamera = std::make_unique<TargetCamera>(m_player.get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, float(directX.GetWidth()) / float(directX.GetHeight()), 0.1f, 10000.0f);
	//グリッド床を生成する
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 200.0f, 50);
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
	m_player->Update(timer);
	// エレメントマネージャの更新
	m_elementManager->Update(timer);

	if (timer.GetFrameCount() % 30 == 0) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 80);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3::One*-3 + area_offset;
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3::One*3 + area_offset;
		m_elementManager->CreateElement(area_start, area_end, 1);
	}

	// 当たり判定
	const SphereCollider* player_collider = m_player->GetCollider();
	for (auto& element : *m_pElements) {
		// 未使用なら飛ばす
		if (!element) {
			continue;
		}
		const SphereCollider* element_collider = element->GetCollider();
		if (player_collider->Collision(element_collider)) {
			element->IsUsed(false);
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
	// モデルを描画する
	m_player->Render(view, projection);
	// エレメントを描画する
	m_elementManager->Render(view, projection);
	
	spriteBatch->End();
}

/// <summary>
/// プレイシーンを終了する
/// </summary>
void PlayScene::Finalize() {

}

