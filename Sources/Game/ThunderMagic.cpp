#include "ThunderMagic.h"
#include <Framework/DirectX11.h>
#include "MagicFactory.h"
#include "MagicManager.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager">魔法マネージャ</param>
ThunderMagic::ThunderMagic(MagicManager* magicManager)
	: Magic(MagicID::Thunder)
	, m_object()
	, m_pMagicManager(magicManager)
	, m_startTimer() 
	, m_time() {
	m_sphereCollider.SetRadius(THUNDER_MAGIC_COLLIDER_RADIUS);
}

/// <summary>
/// デストラクタ
/// </summary>
ThunderMagic::~ThunderMagic() {
}

/// <summary>
/// 雷魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void ThunderMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = float(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	// 落雷生成用タイマーのカウント
	if (m_startTimer) {
		m_time += elapsed_time;
		if (m_time > 0.5f) {
			m_isUsed = false;
		}
	}
	
	DirectX::SimpleMath::Vector3 pos = m_transform.GetPosition();
	pos += m_vel;	
	m_transform.SetPosition(pos);

	// 消滅・タイマーのカウントが達したら落雷魔法を生成する
	if (!m_isUsed) {
		m_pMagicManager->CreateMagic(MagicID::ThunderStrike, m_playerId, pos, DirectX::SimpleMath::Vector3::Down);
	}

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 雷魔法を開放する
/// </summary>
void ThunderMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// 雷魔法を生成する
/// </summary>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="color">色</param>
void ThunderMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), THUNDER_MAGIC_RADIUS);
	m_lifeTime = 5.0f;
	m_startTimer = false;
	m_time = 0.0f;
}

/// <summary>
/// 雷魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void ThunderMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void ThunderMagic::HitPlayer(const SphereCollider& collider) {
	DirectX::SimpleMath::Vector3 direction = collider.GetTransform()->GetPosition() - m_transform.GetPosition();
	// 反対方向なら処理しない
	if (direction.z*m_vel.z < 0) {
		return;
	}
	// 一定距離まで近づいたら即時発射
	if (DirectX::SimpleMath::Vector2(direction.x, direction.z).Length() < 3.0f) {
		m_isUsed = false;
	}
	direction.Normalize();
	m_vel = Math::Lerp(m_vel, direction * THUNDER_MAGIC_CHASE_SPEED, 0.5f);
	// 敵プレイヤーに接近したらタイマー作動
	m_startTimer = true;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void ThunderMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	other_id;
}

