#pragma once
#ifndef COLLISION_DEFINED
#define COLLISION_DEFINED


class Collider;
class SphereCollider;
class CapsuleCollider;


/// <summary>
/// �����蔻�菈���p�N���X
/// </summary>
class Collision {
public:
	// �����蔻����s��
	static bool HitCheck(const Collider* collider1, const Collider* collider2);

	// ���Ƌ��̓����蔻����s��
	static bool HitCheckSphere2Sphere(const SphereCollider& sphere1, const SphereCollider& sphere2);
	// ���ƃJ�v�Z���̓����蔻����s��
	static bool HitCheckSphere2Capsule(const SphereCollider& sphere, const CapsuleCollider& capsule);

public:
	// �_�Ɛ����̋�����2����v�Z����
	static float SqDistPointSegment(const DirectX::SimpleMath::Vector3& p,
		const DirectX::SimpleMath::Vector3& s1, const DirectX::SimpleMath::Vector3& s2);

};


#endif // !COLLISION_DEFINED
