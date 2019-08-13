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
