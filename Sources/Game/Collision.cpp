#include "Collision.h"
#include <Utils\ErrorMessage.h>
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
	// 可能な当たり判定のパターンの定義
	constexpr unsigned int sphere2sphere = static_cast<unsigned int>(Collider::Type::Sphere);
	
	// 適用するパターンを計算する
	unsigned int collision_type = static_cast<unsigned int>(collider1->m_type) | static_cast<unsigned int>(collider2->m_type);

	switch (collision_type) {
	case sphere2sphere:
		return HitCheckSphere2Sphere(*static_cast<const SphereCollider*>(collider1), *static_cast<const SphereCollider*>(collider2));
	default:
		break;
	}
	ErrorMessage(L"無効な当たり判定の組み合わせです");
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
	DirectX::SimpleMath::Vector3 d = sphere1.GetPos() - sphere2.GetPos();
	float dist2 = d.x*d.x + d.y*d.y + d.z*d.z;
	return dist2 <= (sphere1.m_radius + sphere2.m_radius)*(sphere1.m_radius + sphere2.m_radius);

}
