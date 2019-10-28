#include "Collider.h"
#include <Game\Common\Transform.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="type">�����蔻��̎��</param>
/// <param name="pTransform">�p���N���X�ւ̃|�C���^</param>
/// <param name="offset">���W�̃I�t�Z�b�g</param>
Collider::Collider(Type type, const Transform* pTransform, const DirectX::SimpleMath::Vector3& offset) 
	: m_type(type)
	, m_pTransform(pTransform)
	, m_offset(offset) {
}

/// <summary>
/// ���ۂ̍��W���擾����
/// </summary>
/// <returns>
/// ���ۂ̍��W���擾����
/// </returns>
DirectX::SimpleMath::Vector3 Collider::GetPos() const {
	DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Transform(m_offset, m_pTransform->GetRotation());
	return m_pTransform->GetPosition() + offset;
}
