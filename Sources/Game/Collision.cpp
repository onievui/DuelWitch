#include "Collision.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "Transform.h"


/// <summary>
/// 当たり判定を行う
/// </summary>
/// <param name="collider1">当たり判定1</param>
/// <param name="collider2">当たり判定2</param>
/// <returns>
/// true : 当たっている
/// false : 当たっていない
/// </returns>
bool Collision::HitCheck(const Collider* collider1, const Collider* collider2) {
	constexpr unsigned int sphere2sphere = static_cast<unsigned int>(Collider::Type::Sphere);

	unsigned int collision_type = static_cast<unsigned int>(collider1->m_type) | static_cast<unsigned int>(collider2->m_type);

	switch (collision_type) {
	case sphere2sphere:
		return HitCheckSphere2Sphere(*static_cast<const SphereCollider*>(collider1), *static_cast<const SphereCollider*>(collider2));
	default:
		break;
	}
	return false;
}

/// <summary>
/// 球と球の当たり判定を行う
/// </summary>
/// <param name="sphere1">球当たり判定1</param>
/// <param name="sphere2">球当たり判定2</param>
/// <returns>
/// true : 当たっている
/// false : 当たっていない
/// </returns>
bool Collision::HitCheckSphere2Sphere(const SphereCollider& sphere1, const SphereCollider& sphere2) {
	const DirectX::SimpleMath::Vector3& rot = sphere1.m_pTransform->GetRotation();
	DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Transform(sphere1.m_offset,
		DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rot.y, rot.z, rot.z));
	const DirectX::SimpleMath::Vector3& rot2 = sphere2.m_pTransform->GetRotation();
	DirectX::SimpleMath::Vector3 offset2 = DirectX::SimpleMath::Vector3::Transform(sphere2.m_offset,
		DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rot2.y, rot2.z, rot2.z));
	DirectX::SimpleMath::Vector3 d = (sphere1.m_pTransform->GetPosition() + offset) - (sphere2.m_pTransform->GetPosition() + offset2);
	float dist2 = d.x*d.x + d.y*d.y + d.z*d.z;
	return dist2 <= (sphere1.m_radius + sphere2.m_radius)*(sphere1.m_radius + sphere2.m_radius);

}
