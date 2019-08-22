#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include "MagicFactory.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
FreezeMagic::FreezeMagic()
	: Magic(MagicID::Freeze)
	, m_pPlayerPos() {
	m_sphereCollider.SetRadius(FREEZE_MAGIC_RADIUS);
}

/// <summary>
/// デストラクタ
/// </summary>
FreezeMagic::~FreezeMagic() {
}

/// <summary>
/// 氷魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void FreezeMagic::Update(const DX::StepTimer& timer) {
	m_lifeTime -= float(timer.GetElapsedSeconds());
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = *m_pPlayerPos;
	//pos += m_vel;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 氷魔法を開放する
/// </summary>
void FreezeMagic::Lost() {

}

/// <summary>
/// 氷魔法を生成する
/// </summary>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="color">色</param>
void FreezeMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_pPlayerPos = &pos;
	m_color = color;
	m_vel = vel;
	m_lifeTime = 6.0f;
}

/// <summary>
/// 氷魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void FreezeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::FreezeMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void FreezeMagic::HitPlayer(const SphereCollider& collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void FreezeMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// 炎魔法と衝突したら消える
	if (other_id == MagicID::Fire) {
		m_isUsed = false;
	}
}
