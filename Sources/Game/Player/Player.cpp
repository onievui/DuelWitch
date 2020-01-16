#include "Player.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CharaStatusParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Scene\ShareData\ShareData.h>
#include <Game\Command\Command.h>
#include <Game\Command\MoveCommand.h>
#include <Game\Command\AIMoveCommand.h>
#include <Game\Command\CastMagicCommand.h>
#include <Game\Command\AICastMagicCommand.h>
#include <Game\Command\UserRenderCommand.h>
#include <Game\Command\AIRenderCommand.h>
#include <Game\Element\ElementFactory.h>
#include <Game\Magic\MagicID.h>
#include <Game\Magic\IMagic.h>
#include <Game\Magic\MagicManager.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Camera\Camera.h>
#include <Game\Field\FieldData.h>
#include "PlayerID.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="id">プレイヤーID</param>
/// <param name="pos">初期座標</param>
/// <param name="direction">進行方向</param>
Player::Player(PlayerID id, const DirectX::SimpleMath::Vector3& pos)
	: m_id(id)
	, m_status()
	, m_haveElements()
	, m_transform(pos)
	, m_sphereCollider(&m_transform, 0.75f, DirectX::SimpleMath::Vector3(0,0.65f,0)) 
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
/// <param name="pOtherPlayers">他のプレイヤーへのポインタの配列</param>
void Player::Initialize(MagicManager* pMagicManager, Camera* pCamera, std::vector<std::unique_ptr<Player>>& pOtherPlayers) {
	// エフェクトを設定する
	// 箒モデル
	const ModelResource* modelResource = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::Bloom);
	modelResource->GetResource()->UpdateEffects([](DirectX::IEffect* effect) {
		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights) {
			lights->SetLightingEnabled(true);
			lights->SetPerPixelLighting(true);
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
			lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*0.7f);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
	});
	// キャラクターモデル
	const ModelResource* modelResource2 = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::Chara);
	const std::vector<std::unique_ptr<DirectX::Model>>& charas = modelResource2->GetAllResources();
	for (std::vector<std::unique_ptr<DirectX::Model>>::const_iterator itr = charas.begin(); itr != charas.end(); ++itr) {
		(*itr)->UpdateEffects([](DirectX::IEffect* effect) {
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights) {
				lights->SetLightingEnabled(true);
				lights->SetPerPixelLighting(true);
				lights->SetLightEnabled(0, true);
				lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
				lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*0.8f);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
		});
	}

	// ポインタを受け取る
	m_pMagicManager = pMagicManager;
	m_pCamera = pCamera;
	SetOtherPlayers(pOtherPlayers);

	// ステージの中心へ向ける
	DirectX::SimpleMath::Vector3 field_pos = ServiceLocater<FieldData>::Get()->fieldCenter;
	DirectX::SimpleMath::Vector3 field_dir = field_pos - m_transform.GetPosition();
	DirectX::SimpleMath::Quaternion rot = Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::UnitZ, field_dir);
	rot.Inverse(rot);
	m_transform.SetRotation(rot);

	// ステータスを初期化する
	InitializeStatus();

	// コマンドを初期化する
	m_moveCommand->Initialize(*this);
	m_castCommand->Initialize(*this);
	m_renderCommand->Initialize(*this);

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
	
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Z)) {
		ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::Hit, DirectX::SimpleMath::Vector3::Zero, -DirectX::SimpleMath::Vector3::UnitZ);
	}

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
/// 他のプレイヤーを設定する
/// </summary>
/// <param name="pOtherPlayers"></param>
void Player::SetOtherPlayers(std::vector<std::unique_ptr<Player>>& pOtherPlayers) {
	// データをリセットする
	m_pOtherPlayers.clear();
	for (std::vector<std::unique_ptr<Player>>::iterator itr = pOtherPlayers.begin(); itr != pOtherPlayers.end(); itr++) {
		// 自身のポインタは判定しない
		if ((*itr).get() != this) {
			m_pOtherPlayers.push_back((*itr).get());
		}
	}
}

/// <summary>
/// プレイヤーのHPが0以下か取得する
/// </summary>
/// <returns>
/// true : 死んでいる
/// false : 生存している
/// </returns>
bool Player::IsDead() const {
	return m_status.hp <= 0;
}

/// <summary>
/// 魔法のダメージ倍率を取得する
/// </summary>
/// <param name="elementId">エレメントID</param>
/// <returns>
/// ダメージ倍率
/// </returns>
float Player::GetMagicPowerRate(ElementID elementId) const {
	switch (elementId) {
	case ElementID::Fire:
		return m_status.fireMagicPowerRate;
	case ElementID::Freeze:
		return m_status.freezeMagicPowerRate;
	case ElementID::Thunder:
		return m_status.thunderMagicPowerRate;
	default:
		break;
	}

	ErrorMessage(L"魔法のダメージ倍率の取得で不正なIDが渡されました");
	return 0.0f;
}

/// <summary>
/// エレメントの取得処理を行う
/// </summary>
/// <param name="elementId">エレメントID</param>
void Player::GetElement(ElementID elementId) {
	m_haveElements.push_back(elementId);
	// エレメント取得音を鳴らす
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::GetElement);
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
		m_status.hp -= magic->GetPower();
		m_status.damageTimer = 3.0f;

		// ダメージ効果音を鳴らす
		ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Damage);
	}

	// ヒットエフェクトを出現させる
	ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::Hit, m_transform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
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
	const std::vector<int>& chara_id = ServiceLocater<ShareData>::Get()->GetSelectCharaID();
	m_status.charaId = chara_id[static_cast<int>(m_id)];

	const CharaStatusParameter::chara1_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCharaStatusParameter()->chara1Param;

	m_status.maxHp = m_status.hp = m_status.preHp = parameter.maxHp;
	m_status.maxSp = m_status.sp = m_status.preSp = parameter.maxSp;

	m_status.spRecoverySpeed       = parameter.spRecoverySpeed;
	m_status.normalMagicSpCost     = parameter.normalMagicSpCost;
	m_status.boostSpeedRate        = parameter.boostSpeedRate;
	m_status.boostSpCost           = parameter.boostSpCost;
	m_status.rollSpCost            = parameter.rollSpCost;
	m_status.firstChargeTime       = parameter.firstChargeTime;
	m_status.secoundChargeTime     = parameter.secoundChargeTime;
	m_status.fireMagicPowerRate    = parameter.fireMagicPowerRate;
	m_status.freezeMagicPowerRate  = parameter.freezeMagicPowerRate;
	m_status.thunderMagicPowerRate = parameter.thunderMagicPowerRate;

	m_status.damageTimer     = 0.0f;
	m_status.spDecreaseTimer = 0.0f;
	m_status.isBoosting      = false;
	m_status.isCharging      = false;
	m_status.chargeLevel     = 0;
	m_status.canCast         = true;
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
	// 最後にSPを消費してから一定時間経過後に回復し始める
	if (m_status.spDecreaseTimer <= 0) {
		m_status.sp = std::min(m_status.sp + m_status.spRecoverySpeed*elapsed_time, m_status.maxSp);
	}
	m_status.preSp = m_status.sp;
	

	// ダメージ後無敵時間を減らす
	m_status.damageTimer -= elapsed_time;
}

