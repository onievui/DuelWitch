#include "Player.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CharaStatusParameter.h>
#include "PlayParameterLoader.h"
#include "Command.h"
#include "MoveCommand.h"
#include "AIMoveCommand.h"
#include "CastMagicCommand.h"
#include "AICastMagicCommand.h"
#include "UserRenderCommand.h"
#include "AIRenderCommand.h"
#include "MagicID.h"
#include "IMagic.h"
#include "MagicManager.h"
#include "Camera.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="id">プレイヤーID</param>
/// <param name="pos">初期座標</param>
/// <param name="direction">進行方向</param>
Player::Player(PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction)
	: m_id(id)
	, m_status()
	, m_direction(direction)
	, m_haveElements()
	, m_transform(pos, DirectX::SimpleMath::Vector3(0, (m_direction == MoveDirection::Forward ? 0 : Math::PI), 0))
	, m_sphereCollider(&m_transform, 0.75f, DirectX::SimpleMath::Vector3(0,0.5f,0)) 
	, m_pMagicManager()
	, m_pCamera() {

	// プレイヤーと敵プレイヤーでコマンドを変える
	if (id == PlayerID::Player1) {
		m_moveCommand = std::make_unique<MoveCommand>();
		m_castCommand = std::make_unique<CastMagicCommand>();
		m_renderCommand = std::make_unique<UserRenderCommand>();
	}
	else {
		m_moveCommand = std::make_unique<AIMoveCommand>();
		m_castCommand = std::make_unique<AICastMagicCommand>();
		m_renderCommand = std::make_unique<AIRenderCommand>();
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {
}

/// <summary>
/// プレイヤーを初期化する
/// </summary>
/// <param name="pMagicManager">魔法マネージャへのポインタ</param>
/// <param name="pCamera">カメラへのポインタ</param>
/// <param name="pOtherPlayer">敵プレイヤーへのポインタ</param>
void Player::Initialize(MagicManager* pMagicManager, Camera* pCamera, Player* pOtherPlayer) {
	// エフェクトを設定する
	const ModelResource* modelResource = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::BloomModel);
	modelResource->GetResource()->UpdateEffects([](DirectX::IEffect* effect) {
		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights) {
			lights->SetLightingEnabled(true);
			lights->SetPerPixelLighting(true);
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
			lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*0.3f);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
	});

	// ポインタを受け取る
	m_pMagicManager = pMagicManager;
	m_pCamera = pCamera;
	m_pOtherPlayer = pOtherPlayer;

	// ステータスを初期化する
	InitializeStatus();

	// コマンドを初期化する
	m_moveCommand->Initialize(*this);
	m_castCommand->Initialize(*this);
	m_renderCommand->Initialize(*this);

	//ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::PlayerTrail, m_transform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ)
	//	->SetParent(&m_transform);
	//if (m_id == PlayerID::Player1) {
	//	static Transform tra;
	//	tra.SetPosition(DirectX::SimpleMath::Vector3(0, 3, 40));
	//	ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::FireMagic, m_transform.GetPosition(), DirectX::SimpleMath::Vector3::UnitX)
	//		->SetParent(&tra);
	//}

}


/// <summary>
/// プレイヤーを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Player::Update(const DX::StepTimer& timer) {
	// 移動を行う
	m_moveCommand->Execute(*this, timer);
	// 魔法を発動する
	m_castCommand->Execute(*this, timer);
	// 描画のための処理を行う
	m_renderCommand->Execute(*this, timer);

	// ステータスを更新する
	UpdateStatus(timer);
	
}

/// <summary>
/// プレイヤーを解放する
/// </summary>
void Player::Lost() {

}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	proj, view;
}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
	m_renderCommand->Render(*this, view, proj, spriteBatch);
}

/// <summary>
/// プレイヤーの行列を取得する
/// </summary>
/// <returns>
/// プレイヤーの行列
/// </returns>
const DirectX::SimpleMath::Matrix& Player::GetMatrix() const {
	return m_world;
}

/// <summary>
/// プレイヤーの当たり判定を取得する
/// </summary>
/// <returns>
/// 当たり判定
/// </returns>
const Collider* Player::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// プレイヤーIDを取得する
/// </summary>
/// <returns>
/// プレイヤーID
/// </returns>
PlayerID Player::GetPlayerID() const {
	return m_id;
}

/// <summary>
/// エレメントの取得処理を行う
/// </summary>
/// <param name="elementId">エレメントID</param>
void Player::GetElement(ElementID elementId) {
	m_haveElements.push_back(elementId);
}

/// <summary>
/// プレイヤー同士の衝突処理を行う
/// </summary>
/// <param name="player">相手プレイヤー</param>
void Player::HitPlayer(const Player& player) {
	constexpr float reflect_distance = 0.25f;

	DirectX::SimpleMath::Vector3 my_pos = m_transform.GetLocalPosition();
	const DirectX::SimpleMath::Vector3& other_pos = player.m_transform.GetLocalPosition();
	// 相手プレイヤーと判定方向に進む
	float angle = std::atan2f(my_pos.y - other_pos.y, my_pos.x - other_pos.x);
	my_pos += DirectX::SimpleMath::Vector3(std::cosf(angle), std::sinf(angle), 0.0f)*reflect_distance;
	m_transform.SetPosition(my_pos);
}

/// <summary>
/// 魔法との衝突処理を行う
/// </summary>
/// <param name="magic">魔法</param>
void Player::HitMagic(const IMagic* magic) {
	if (magic->GetID() == MagicID::Thunder) {
		return;
	}
	// 無敵時間でなければダメージを受ける
	if (m_status.damageTimer <= 0.0f) {
		m_status.hp -= 10.0f;
		m_status.damageTimer = 3.0f;
	}
}

/// <summary>
/// フィールドとの衝突判定を行う
/// </summary>
/// <param name="hitPos">衝突点</param>
void Player::HitField(const DirectX::SimpleMath::Vector3& hitPos) {
	DirectX::SimpleMath::Vector3 change = hitPos - m_sphereCollider.GetPos();
	m_transform.SetPosition(m_transform.GetLocalPosition() + change);
}

/// <summary>
/// ステータスを初期化する
/// </summary>
void Player::InitializeStatus() {
	const CharaStatusParameter::chara1_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCharaStatusParameter()->chara1Param;

	m_status.maxHp = m_status.hp = m_status.preHp = parameter.maxHp;
	m_status.maxSp = m_status.sp = m_status.preSp = parameter.maxSp;
	m_status.spRecoverySpeed = parameter.spRecoverySpeed;
	m_status.normalMagicSpCost = parameter.normalMagicSpCost;
	m_status.boostSpeedRate = parameter.boostSpeedRate;
	m_status.boostSpCost = parameter.boostSpCost;

	m_status.damageTimer = 0.0f;
	m_status.spDecreaseTimer = 0.0f;
	m_status.isBoosting = false;
}

/// <summary>
/// ステータスを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Player::UpdateStatus(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_status.preHp = m_status.hp;

	// SPを回復する
	if (m_status.preSp > m_status.sp) {
		// SPを消費した直後は回復しない
		const float sp_decrease_time = 2.0f;
		m_status.spDecreaseTimer = sp_decrease_time;
	}
	else {
		m_status.spDecreaseTimer -= elapsed_time;
	}
	if (m_status.spDecreaseTimer <= 0) {
		m_status.sp = std::min(m_status.sp + m_status.spRecoverySpeed*elapsed_time, m_status.maxSp);
	}
	m_status.preSp = m_status.sp;
	

	// ダメージ後無敵時間を減らす
	m_status.damageTimer -= elapsed_time;
}

