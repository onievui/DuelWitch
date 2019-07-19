#include "ThunderMagic.h"
#include <Framework/DirectX11.h>
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
ThunderMagic::ThunderMagic()
	: Magic()
	, m_object() {
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
	m_lifeTime -= float(timer.GetElapsedSeconds());
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetPosition();
	pos += m_vel;	
	m_transform.SetPosition(pos);

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
	auto direction = collider.GetTransform()->GetPosition() - m_transform.GetPosition();
	direction.Normalize();
	m_vel = Math::Lerp(m_vel, direction * THUNDER_MAGIC_CHASE_SPEED, 0.5f);
}
