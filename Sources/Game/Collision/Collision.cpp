#include "Collision.h"
#include <Utils\ErrorMessage.h>
#include "Collider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include <Game\Object\Transform.h>


/// <summary>
/// �����蔻����s��
/// </summary>
/// <param name="collider1">�����蔻��1</param>
/// <param name="collider2">�����蔻��2</param>
/// <returns>
/// true : �������Ă���
/// false : �������Ă��Ȃ�
/// </returns>
bool Collision::HitCheck(const Collider* collider1, const Collider* collider2) {
	// �\�ȓ����蔻��̃p�^�[���̒�`
	constexpr unsigned int sphere2sphere = static_cast<unsigned int>(Collider::Type::Sphere);
	constexpr unsigned int sphere2capsule = static_cast<unsigned int>(Collider::Type::Sphere) |
		static_cast<unsigned int>(Collider::Type::Capsule);
	constexpr unsigned int capsule2capsule = static_cast<unsigned int>(Collider::Type::Capsule);
	
	// �K�p����p�^�[�����v�Z����
	unsigned int collision_type = static_cast<unsigned int>(collider1->m_type) | static_cast<unsigned int>(collider2->m_type);

	switch (collision_type) {
	// �����m�̓����蔻��
	case sphere2sphere:
		return HitCheckSphere2Sphere(*static_cast<const SphereCollider*>(collider1), *static_cast<const SphereCollider*>(collider2));
	// ���ƃJ�v�Z���̓����蔻��
	case sphere2capsule:
		if (collider1->m_type == Collider::Type::Sphere) {
			return HitCheckSphere2Capsule(*static_cast<const SphereCollider*>(collider1), *static_cast<const CapsuleCollider*>(collider2));
		}
		return HitCheckSphere2Capsule(*static_cast<const SphereCollider*>(collider2), *static_cast<const CapsuleCollider*>(collider1));
	// �J�v�Z���ƃJ�v�Z���̓����蔻��
	case capsule2capsule:
		// ����K�v�Ȃ�
		return false;
	default:
		break;
	}
	ErrorMessage(L"�����ȓ����蔻��̑g�ݍ��킹�ł�");
	return false;
}

/// <summary>
/// ���Ƌ��̓����蔻����s��
/// </summary>
/// <param name="sphere1">�������蔻��1</param>
/// <param name="sphere2">�������蔻��2</param>
/// <returns>
/// true : �������Ă���
/// false : �������Ă��Ȃ�
/// </returns>
bool Collision::HitCheckSphere2Sphere(const SphereCollider& sphere1, const SphereCollider& sphere2) {
	DirectX::SimpleMath::Vector3 d = sphere1.GetPos() - sphere2.GetPos();
	float dist2 = d.x*d.x + d.y*d.y + d.z*d.z;
	return dist2 <= (sphere1.m_radius + sphere2.m_radius)*(sphere1.m_radius + sphere2.m_radius);

}

/// <summary>
/// ���ƃJ�v�Z���̓����蔻����s��
/// </summary>
/// <param name="sphere">�������蔻��</param>
/// <param name="capsule">�J�v�Z�������蔻��</param>
/// <returns>
/// true : �������Ă���
/// false : �������Ă��Ȃ�
/// </returns>
bool Collision::HitCheckSphere2Capsule(const SphereCollider& sphere, const CapsuleCollider& capsule) {
	// ���̒��S�ƃJ�v�Z���̐����Ƃ̋�����2����v�Z
	float dist2 = SqDistPointSegment(sphere.GetPos(), capsule.GetStartPos(), capsule.GetEndPos());
	float radius = sphere.m_radius + capsule.m_radius;
	return dist2 <= radius * radius;
}

/// <summary>
/// �_�Ɛ����̋�����2����v�Z����
/// </summary>
/// <param name="p">�_�̍��W</param>
/// <param name="s1">�����̎n�_</param>
/// <param name="s2">�����̏I�_</param>
/// <returns>
/// �_�Ɛ����̋�����2��
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
