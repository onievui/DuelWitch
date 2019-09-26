#include "Camera.h"


/// <summary>
/// �����x�N�g�����擾����
/// </summary>
/// <returns>
/// �J�����̕����x�N�g��
/// </returns>
DirectX::SimpleMath::Vector3 Camera::GetEyeVector() const {
	DirectX::SimpleMath::Vector3 vec = m_target - m_eye;
	vec.Normalize();
	return vec;
}

/// <summary>
/// �X�N���[�����W���烌�C�ɕϊ�����
/// </summary>
/// <param name="point">�X�N���[�����W</param>
/// <returns>
/// ���C
/// </returns>
DirectX::SimpleMath::Ray Camera::ScreenPointToRay(const DirectX::SimpleMath::Vector3& point) const {
	return ViewportPointToRay(ScreenToViewportPoint(point));
}

/// <summary>
/// �r���[�|�[�g���W���烌�C�ɕϊ�����
/// </summary>
/// <param name="point">�r���[�|�[�g���W</param>
/// <returns>
/// ���C
/// </returns>
DirectX::SimpleMath::Ray Camera::ViewportPointToRay(const DirectX::SimpleMath::Vector3& point) const {
	DirectX::SimpleMath::Vector3 point_near = DirectX::SimpleMath::Vector3(point.x, point.y, 0.0f);
	DirectX::SimpleMath::Vector3 point_far = DirectX::SimpleMath::Vector3(point.x, point.y, 1.0f);

	DirectX::SimpleMath::Matrix inverse_viewproj = (m_view * m_proj).Invert();
	DirectX::SimpleMath::Vector3 ray_near = DirectX::SimpleMath::Vector3::Transform(point_near, inverse_viewproj);
	DirectX::SimpleMath::Vector3 ray_far = DirectX::SimpleMath::Vector3::Transform(point_far, inverse_viewproj);
	DirectX::SimpleMath::Vector3 ray_direction = ray_far - ray_near;
	ray_direction.Normalize();

	return DirectX::SimpleMath::Ray(ray_near, ray_direction);
}

/// <summary>
/// ���[���h���W����X�N���[�����W�ɕϊ�����
/// </summary>
/// <param name="point">���[���h���W</param>
/// <returns>
/// �X�N���[�����W
/// </returns>
DirectX::SimpleMath::Vector3 Camera::WorldToScreenPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_view * m_proj * m_viewport);
}

/// <summary>
/// ���[���h���W����r���[�|�[�g���W�ɕϊ�����
/// </summary>
/// <param name="point">���[���h���W</param>
/// <returns>
/// �r���[�|�[�g���W
/// </returns>
DirectX::SimpleMath::Vector3 Camera::WorldToViewportPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_view * m_proj);
}

/// <summary>
/// �r���[�|�[�g���W���烏�[���h���W�ɕϊ�����
/// </summary>
/// <param name="point">�r���[�|�[�g���W</param>
/// <returns>
/// ���[���h���W
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ViewportToWorldPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, (m_view* m_proj).Invert());
}

/// <summary>
/// �X�N���[�����W���烏�[���h���W�ɕϊ�����
/// </summary>
/// <param name="point">�X�N���[�����W</param>
/// <returns>
/// ���[���h���W
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ScreenToWorldPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, (m_view* m_proj * m_viewport).Invert());
}

/// <summary>
/// �X�N���[�����W����r���[�|�[�g���W�ɕϊ�����
/// </summary>
/// <param name="point">�X�N���[�����W</param>
/// <returns>
/// �r���[�|�[�g���W
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ScreenToViewportPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_viewport.Invert());
}

/// <summary>
/// �r���[�|�[�g���W����X�N���[�����W�ɕϊ�����
/// </summary>
/// <param name="point">�r���[�|�[�g���W</param>
/// <returns>
/// �X�N���[�����W
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ViewportToScreenPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_viewport);
}
