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
#include <Game\Effect\DeathEffectEmitter.h>
#include <Game\Camera\Camera.h>
#include <Game\Field\FieldData.h>
#include <Game\UI\EventLogger.h>
#include "PlayerID.h"
#include "PlayerData.h"


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
	, m_sphereCollider(&m_transform, COLLIDER_RADIUS, DirectX::SimpleMath::Vector3(0,COLLIDER_OFFSET_Y,0))
	, m_lockOnCollider(&m_transform, LOCKON_COLLIDER_RADIUS, DirectX::SimpleMath::Vector3(0,COLLIDER_OFFSET_Y,0))
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
	ServiceLocater<PlayerData>::Get()->transforms[static_cast<int>(m_id)] = nullptr;
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
	for (unsigned int i = 0; i < charas.size(); ++i) {
		// unityちゃんモデルは元から明るいため、環境光を控えめにする
		float ambient_light_power = (i == 0 ? 0.5f : 0.8f);

		charas[i]->UpdateEffects([ambient_light_power](DirectX::IEffect* effect) {
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights) {
				lights->SetLightingEnabled(true);
				lights->SetPerPixelLighting(true);
				lights->SetLightEnabled(0, true);
				lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
				lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*ambient_light_power);
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

	// 外部から姿勢を参照できるようにする
	ServiceLocater<PlayerData>::Get()->transforms[static_cast<int>(m_id)] = &m_transform;
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
		//ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::Death, DirectX::SimpleMath::Vector3::Zero, -DirectX::SimpleMath::Vector3::UnitZ);
	}

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
/// ロックオン用当たり判定を取得する
/// </summary>
/// <returns>
/// ロックオン用当たり判定
/// </returns>
const Collider* Player::GetLockOnCollider() const {
	return &m_lockOnCollider;
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
/// プレイヤーがブースト移動中か取得する
/// </summary>
/// <returns>
/// true : ブースト移動中
/// false : ブースト移動していない
/// </returns>
bool Player::IsBoosting() const {
	return m_status.isBoosting;
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
	// エレメントをこれ以上持てない場合は取得しない
	if (m_haveElements.size() < HAVE_ELEMENT_MAX) {
		m_haveElements.push_back(elementId);
	}
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

		// 体力が0になった時はエフェクトを出す
		if (m_status.hp <= 0.0f) {
			EffectManager* effect_manager = ServiceLocater<EffectManager>::Get();
			IEffectEmitter* effect = effect_manager->CreateEffect(EffectID::Death, m_transform.GetPosition(), DirectX::SimpleMath::Vector3::Zero);
			DeathEffectEmitter* death_effect = dynamic_cast<DeathEffectEmitter*>(effect);
			if (death_effect) {
				death_effect->SetColorID(m_status.charaId);
			}
			// イベントログで通知する
			int log_id = static_cast<int>(EventLogID::Player1Dead) + static_cast<int>(m_id);
			ServiceLocater<EventLogger>::Get()->Log(static_cast<EventLogID>(log_id));
		}
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

	const CharaStatusParameter::chara_param& parameter =
		ServiceLocater<PlayParameterLoader>::Get()->GetCharaStatusParameter()->charaParams[m_status.charaId];

	m_status.maxHp = m_status.hp = m_status.preHp = parameter.maxHp;
	m_status.maxSp = m_status.sp = m_status.preSp = parameter.maxSp;

	m_status.spRecoverySpeed       = parameter.spRecoverySpeed;
	m_status.normalMagicSpCost     = parameter.normalMagicSpCost;
	m_status.boostSpeedRate        = parameter.boostSpeedRate;
	m_status.boostSpCost           = parameter.boostSpCost;
	m_status.rollSpCost            = parameter.rollSpCost;
	m_status.quickTurnSpCost       = parameter.quickTurnSpCost;
	m_status.firstChargeTime       = parameter.firstChargeTime;
	m_status.secoundChargeTime     = parameter.secoundChargeTime;
	m_status.fireMagicPowerRate    = parameter.fireMagicPowerRate;
	m_status.freezeMagicPowerRate  = parameter.freezeMagicPowerRate;
	m_status.thunderMagicPowerRate = parameter.thunderMagicPowerRate;

	m_status.damageTimer     = 0.0f;
	m_status.spDecreaseTimer = 0.0f;
	m_status.isBoosting      = false;
	m_status.lockOnPlayerID  = -1;
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

