#pragma once
#ifndef COLLISION_DEFINED
#define COLLISION_DEFINED


class Collider;
class SphereCollider;
class CapsuleCollider;


/// <summary>
/// 当たり判定処理用クラス
/// </summary>
class Collision {
public:
	// 当たり判定を行う
	static bool HitCheck(const Collider* collider1, const Collider* collider2);

	// 球と球の当たり判定を行う
	static bool HitCheckSphere2Sphere(const SphereCollider& sphere1, const SphereCollider& sphere2);
	// 球とカプセルの当たり判定を行う
	static bool HitCheckSphere2Capsule(const SphereCollider& sphere, const CapsuleCollider& capsule);

public:
	// 点と線分の距離の2乗を計算する
	static float SqDistPointSegment(const DirectX::SimpleMath::Vector3& p,
		const DirectX::SimpleMath::Vector3& s1, const DirectX::SimpleMath::Vector3& s2);

};


#endif // !COLLISION_DEFINED
