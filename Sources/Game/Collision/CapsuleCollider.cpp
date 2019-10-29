#include "CapsuleCollider.h"
#include <Game\Object\Transform.h>


/// <summary>
/// �@�R���X�g���N�^
/// </summary>
/// <param name="pTransform">�p���ւ̃|�C���^</param>
/// <param name="radius">���a</param>
/// <param name="startPos">�n�_</param>
/// <param name="endPos">�I�_</param>
/// <param name="offset">���W�̃I�t�Z�b�g</param>
CapsuleCollider::CapsuleCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& startPos,
	const DirectX::SimpleMath::Vector3& endPos, const DirectX::SimpleMath::Vector3& offset)
	: Collider(Collider::Type::Capsule, pTransform,offset)
	, m_radius(radius) {
	m_pos[0] = startPos;
	m_pos[1] = endPos;
}

/// <summary>
/// ���ۂ̎n�_���擾����
/// </summary>
/// <returns>
/// ���ۂ̎n�_
/// </returns>
DirectX::SimpleMath::Vector3 CapsuleCollider::GetStartPos() const {
	Transform tmp = *m_pTransform;
	tmp.SetPosition(tmp.GetLocalPosition() + m_pos[0]);
	DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Transform(m_offset, m_pTransform->GetRotation());
	return tmp.GetPosition() + offset;
}

/// <summary>
/// ���ۂ̏I�_���擾����
/// </summary>
/// <returns>
/// ���ۂ̏I�_
/// </returns>
DirectX::SimpleMath::Vector3 CapsuleCollider::GetEndPos() const {
	Transform tmp = *m_pTransform;
	tmp.SetPosition(tmp.GetLocalPosition() + m_pos[1]);
	DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Transform(m_offset, m_pTransform->GetRotation());
	return tmp.GetPosition() + offset;
}
