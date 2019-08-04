#pragma once
#ifndef COLLISION_DEFINED
#define COLLISION_DEFINED


class Collider;
class SphereCollider;


/// <summary>
/// �����蔻�菈���p�N���X
/// </summary>
class Collision {
public:
	// �����蔻����s��
	static bool HitCheck(const Collider* collider1, const Collider* collider2);

	// ���Ƌ��̓����蔻����s��
	static bool HitCheckSphere2Sphere(const SphereCollider& sphere1, const SphereCollider& sphere2);

};


#endif // !COLLISION_DEFINED
