#include "Collision.h"
#include <Utils\ErrorMessage.h>
#include "Collider.h"
#include "SphereCollider.h"
#include "Transform.h"


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
	
	// �K�p����p�^�[�����v�Z����
	unsigned int collision_type = static_cast<unsigned int>(collider1->m_type) | static_cast<unsigned int>(collider2->m_type);

	switch (collision_type) {
	case sphere2sphere:
		return HitCheckSphere2Sphere(*static_cast<const SphereCollider*>(collider1), *static_cast<const SphereCollider*>(collider2));
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
