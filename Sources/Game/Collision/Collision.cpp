#include "Collision.h"
#include <Utils\ErrorMessage.h>
#include "Collider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include <Game\Object\Transform.h>


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
	constexpr unsigned int sphere2capsule = static_cast<unsigned int>(Collider::Type::Sphere) |
		static_cast<unsigned int>(Collider::Type::Capsule);
	constexpr unsigned int capsule2capsule = static_cast<unsigned int>(Collider::Type::Capsule);
	
	// 適用するパターンを計算する
	unsigned int collision_type = static_cast<unsigned int>(collider1->m_type) | static_cast<unsigned int>(collider2->m_type);

	switch (collision_type) {
	// 球同士の当たり判定
	case sphere2sphere:
		return HitCheckSphere2Sphere(*static_cast<const SphereCollider*>(collider1), *static_cast<const SphereCollider*>(collider2));
	// 球とカプセルの当たり判定
	case sphere2capsule:
		if (collider1->m_type == Collider::Type::Sphere) {
			return HitCheckSphere2Capsule(*static_cast<const SphereCollider*>(collider1), *static_cast<const CapsuleCollider*>(collider2));
		}
		return HitCheckSphere2Capsule(*static_cast<const SphereCollider*>(collider2), *static_cast<const CapsuleCollider*>(collider1));
	// カプセルとカプセルの当たり判定
	case capsule2capsule:
		// 現状必要なし
		return false;
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

/// <summary>
/// 球とカプセルの当たり判定を行う
/// </summary>
/// <param name="sphere">球当たり判定</param>
/// <param name="capsule">カプセル当たり判定</param>
/// <returns>
/// true : 当たっている
/// false : 当たっていない
/// </returns>
bool Collision::HitCheckSphere2Capsule(const SphereCollider& sphere, const CapsuleCollider& capsule) {
	// 球の中心とカプセルの線分との距離の2乗を計算
	float dist2 = SqDistPointSegment(sphere.GetPos(), capsule.GetStartPos(), capsule.GetEndPos());
	float radius = sphere.m_radius + capsule.m_radius;
	return dist2 <= radius * radius;
}

/// <summary>
/// 点と線分の距離の2乗を計算する
/// </summary>
/// <param name="p">点の座標</param>
/// <param name="s1">線分の始点</param>
/// <param name="s2">線分の終点</param>
/// <returns>
/// 点と線分の距離の2乗
/// </returns>
float Collision::SqDistPointSegment(const DirectX::SimpleMath::Vector3& p, const DirectX::SimpleMath::Vector3& s1,
	const DirectX::SimpleMath::Vector3& s2) {
	DirectX::SimpleMath::Vector3 s1s2 = s2 - s1;
	DirectX::SimpleMath::Vector3 s1p = p - s1;
	DirectX::SimpleMath::Vector3 s2p = p - s2;
	float e = s1p.Dot(s1s2);
	if (e <= 0.0f) {
		return s1p.x*s1p.x + s1p.y*s1p.y;
	}
	float f = s1s2.x*s1s2.x + s1s2.y*s1s2.y;
	if (e >= f) {
		return s2p.x*s2p.x + s2p.y*s2p.y;
	}
	return s1p.Dot(s1p) - e * e / f;
}
