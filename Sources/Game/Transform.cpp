#include "Transform.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="rotation">��]</param>
/// <param name="scale">�X�P�[��</param>
Transform::Transform(const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& rotation,
	const DirectX::SimpleMath::Vector3& scale)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(scale) {

}

/// <summary>
/// ���W���擾����
/// </summary>
/// <returns>
/// ���W
/// </returns>
const DirectX::SimpleMath::Vector3& Transform::GetPosition() const {
	return m_position;
}

/// <summary>
/// ���W��ݒ肷��
/// </summary>
/// <param name="pos">���W</param>
void Transform::SetPosition(const DirectX::SimpleMath::Vector3& position) {
	m_position = position;
}

/// <summary>
/// ��]���擾����
/// </summary>
/// <returns>
/// ��]
/// </returns>
const DirectX::SimpleMath::Vector3& Transform::GetRotation() const {
	return m_rotation;
}

/// <summary>
/// ��]��ݒ肷��
/// </summary>
/// <param name="rotation">��]</param>
void Transform::SetRotation(const DirectX::SimpleMath::Vector3& rotation) {
	m_rotation = rotation;
}

/// <summary>
/// �X�P�[�����擾����
/// </summary>
/// <returns>
/// �X�P�[��
/// </returns>
const DirectX::SimpleMath::Vector3& Transform::GetScale() const {
	return m_scale;
}

/// <summary>
/// �X�P�[����ݒ肷��
/// </summary>
/// <param name="scale">�X�P�[��</param>
void Transform::SetScale(const DirectX::SimpleMath::Vector3& scale) {
	m_scale = scale;
}

/// <summary>
/// ���[���h�s����擾����
/// </summary>
/// <returns>
/// ���[���h�s��
/// </returns>
DirectX::SimpleMath::Matrix Transform::GetMatrix() const {
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	DirectX::SimpleMath::Quaternion quaternion = 
		DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
	world *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	return world;
}
