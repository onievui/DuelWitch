#pragma once
#ifndef COLLISION_DEFINED
#define COLLISION_DEFINED


class Collider;
class SphereCollider;


/// <summary>
/// 当たり判定処理用クラス
/// </summary>
class Collision {
public:
	// 当たり判定を行う
	static bool HitCheck(const Collider* collider1, const Collider* collider2);

	// 球と球の当たり判定を行う
	static bool HitCheckSphere2Sphere(const SphereCollider& sphere1, const SphereCollider& sphere2);

};


#endif // !COLLISION_DEFINED
