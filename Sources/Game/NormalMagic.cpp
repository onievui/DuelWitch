#include "NormalMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
NormalMagic::NormalMagic()
	: Magic(MagicID::Normal) {
	m_sphereCollider.SetRadius(ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->normalParam.radius);
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
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetPosition();
	pos += m_vel*elapsed_time;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 通常魔法を開放する
/// </summary>
void NormalMagic::Lost() {

}

/// <summary>
/// 通常魔法を生成する
/// </summary>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="dir">方向</param>
/// <param name="color">色</param>
void NormalMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
	const DirectX::SimpleMath::Vector4& color) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_sphereCollider.SetRadius(parameter->normalParam.radius);
	m_color = color;
	m_vel = dir*parameter->normalParam.moveSpeed;
	m_lifeTime = parameter->normalParam.lifeTime;
}

/// <summary>
/// 通常魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void NormalMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::NormalMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void NormalMagic::HitPlayer(const Collider* collider) {
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
