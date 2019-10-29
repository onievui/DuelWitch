#include "Transform.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="rotation">��]</param>
/// <param name="scale">�X�P�[��</param>
Transform::Transform(const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(scale) {
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="euler">�I�C���[�p</param>
/// <param name="scale">�X�P�[��</param>
Transform::Transform(const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& euler,
	const DirectX::SimpleMath::Vector3& scale) 
	: m_position(position)
	, m_rotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z))
	, m_scale(scale) {
}

/// /// <summary>
/// ���[���h�s����擾����
/// </summary>
/// <returns>
/// ���[���h�s��
/// </returns>
DirectX::SimpleMath::Matrix Transform::GetMatrix() const {
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	world *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	return world;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pParent">�e�I�u�W�F�N�g�ւ̃|�C���^</param>
/// <param name="position">���W</param>
/// <param name="rotation">��]</param>
/// <param name="scale">�X�P�[��</param>
ChildTransform::ChildTransform(const Transform* pParent, const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation, const DirectX::SimpleMath::Vector3& scale)
	: Transform(position, rotation, scale)
	, m_pParent(pParent) {
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pParent">�e�I�u�W�F�N�g�ւ̃|�C���^</param>
/// <param name="position">���W</param>
/// <param name="euler">�I�C���[�p</param>
/// <param name="scale">�X�P�[��</param>
ChildTransform::ChildTransform(const Transform* pParent, const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& euler, const DirectX::SimpleMath::Vector3& scale)
	: Transform(position, euler, scale)
	, m_pParent(pParent) {
}

/// <summary>
/// ���W���擾����
/// </summary>
/// <returns>
/// ���W
/// </returns>
const DirectX::SimpleMath::Vector3 ChildTransform::GetPosition() const {
	if (m_pParent) {
		return DirectX::SimpleMath::Vector3::Transform(m_position, m_pParent->GetMatrix());
	}
	return GetLocalPosition();
}

/// <summary>
/// ��]���擾����
/// </summary>
/// <returns>
/// ��]
/// </returns>
const DirectX::SimpleMath::Quaternion ChildTransform::GetRotation() const {
	if (m_pParent) {
		return m_rotation * m_pParent->GetRotation();
	}
	return GetLocalRotation();
}

/// <summary>
/// �X�P�[�����擾����
/// </summary>
/// <returns>
/// �X�P�[��
/// </returns>
const DirectX::SimpleMath::Vector3 ChildTransform::GetScale() const {
	if (m_pParent) {
		return m_scale * m_pParent->GetScale();
	}
	return GetLocalScale();
}

/// <summary>
/// ���[���h�s����擾����
/// </summary>
/// <returns>
/// ���[���h�s��
/// </returns>
DirectX::SimpleMath::Matrix ChildTransform::GetMatrix() const {
	if (m_pParent) {
		return Transform::GetMatrix()*m_pParent->GetMatrix();
	}
	return Transform::GetMatrix();
}

/// <summary>
/// �e�I�u�W�F�N�g��ݒ肷��
/// </summary>
/// <param name="pParent">�e�I�u�W�F�N�g�ւ̃|�C���^</param>
void ChildTransform::SetParent(const Transform* pParent) {
	m_pParent = pParent;
}
