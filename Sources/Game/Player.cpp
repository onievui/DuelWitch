#include "Player.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include "Command.h"
#include "MoveCommand.h"
#include "AIMoveCommand.h"
#include "CastMagicCommand.h"
#include "AICastMagicCommand.h"
#include "MagicID.h"
#include "IMagic.h"
#include "MagicManager.h"
#include "Camera.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager">魔法マネージャ</param>
/// <param name="id">プレイヤーID</param>
/// <param name="pos">初期座標</param>
/// <param name="direction">進行方向</param>
Player::Player(MagicManager* magicManager, PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction)
	: m_states()
	, m_id(id)
	, m_direction(direction)
	, m_haveElements()
	, m_transform(pos, DirectX::SimpleMath::Vector3(0, (m_direction == MoveDirection::Forward ? 0 : Math::PI), 0))
	, m_sphereCollider(&m_transform, 0.75f, DirectX::SimpleMath::Vector3(0,0.5f,0)) 
	, m_pMagicManager(magicManager)
	, m_pCamera() {
	if (id == PlayerID::Player1) {
		m_moveCommand = std::make_unique<MoveCommand>();
		m_castCommand = std::make_unique<CastMagicCommand>();
	}
	else {
		m_moveCommand = std::make_unique<AIMoveCommand>();
		m_castCommand = std::make_unique<AICastMagicCommand>();
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {
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

	m_damageTimer -= static_cast<float>(timer.GetElapsedSeconds());
}

/// <summary>
/// プレイヤーを解放する
/// </summary>
void Player::Lost() {
	m_states.reset();
}

/// <summary>
/// プレイヤーを生成する
/// </summary>
void Player::Create() {
	// デバイスの取得
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();

	// コモンステートを作成する
	m_states = std::make_unique<DirectX::CommonStates>(device);

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

}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	if (m_damageTimer <= 0.0f || sin(m_damageTimer*Math::PI2*2)>0) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::BloomModel)->GetResource();
		model->Draw(context, *m_states, m_transform.GetMatrix(), view, proj);
		m_sphereCollider.Render(view, proj);
	}
}

void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	if (m_damageTimer <= 0.0f || sin(m_damageTimer*Math::PI2 * 2) > 0) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::BloomModel)->GetResource();
		model->Draw(context, *m_states, m_transform.GetMatrix(), view, proj);
		m_sphereCollider.Render(view, proj);
	}
	if (m_id == PlayerID::Player2)
		return;

	int i = m_haveElements.size() - 1;
	for (std::list<ElementID>::const_reverse_iterator itr = m_haveElements.rbegin(); itr != m_haveElements.rend(); ++itr) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);
		spriteBatch->Draw(texture->GetResource(static_cast<int>(*itr)).Get(), DirectX::SimpleMath::Vector2(20 + i * 40.0f, 630.0f), nullptr,
			DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2(1.5f, 1.5f));
		--i;
	}

	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicAiming);
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();
	spriteBatch->Draw(texture->GetResource().Get(),
		mouse_pos, nullptr, DirectX::SimpleMath::Vector4(1,1,1,0.8f), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.25f, 0.25f));
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
/// 敵プレイヤーを設定する
/// </summary>
/// <param name="otherPlayer">敵プレイヤーへのポインタ</param>
void Player::SetOtherPlayer(Player* otherPlayer) {
	m_otherPlayer = otherPlayer;
}

/// <summary>
/// カメラを設定する
/// </summary>
/// <param name="camera">カメラへのポインタ</param>
void Player::SetCamera(Camera* camera) {
	m_pCamera = camera;
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

	DirectX::SimpleMath::Vector3 my_pos = m_transform.GetPosition();
	const DirectX::SimpleMath::Vector3& other_pos = player.m_transform.GetPosition();
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
	if (m_damageTimer <= 0.0f) {
		m_damageTimer = 3.0f;
	}
}

