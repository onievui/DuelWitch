#include "NormalMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include "MagicFactory.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
NormalMagic::NormalMagic()
	: Magic(MagicID::Normal)
	, m_object() {
	m_sphereCollider.SetRadius(NORMAL_MAGIC_RADIUS);
}

/// <summary>
/// デストラクタ
/// </summary>
NormalMagic::~NormalMagic() {
}

/// <summary>
/// 通常魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void NormalMagic::Update(const DX::StepTimer& timer) {
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
/// 通常魔法を開放する
/// </summary>
void NormalMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// 通常魔法を生成する
/// </summary>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="color">色</param>
void NormalMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(ServiceLocater<DirectX11>::Get()->GetContext().Get(), NORMAL_MAGIC_RADIUS);
	m_lifeTime = 10.0f;
}

/// <summary>
/// 通常魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void NormalMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void NormalMagic::HitPlayer(const SphereCollider& collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void NormalMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// 氷魔法と衝突したら消える
	if (other_id == MagicID::Freeze) {
		m_isUsed = false;
	}
}
