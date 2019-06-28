#include "Camera.h"


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
	auto point_near = DirectX::SimpleMath::Vector3(point.x, point.y, 0.0f);
	auto point_far = DirectX::SimpleMath::Vector3(point.x, point.y, 1.0f);

	auto inverse_viewproj = (m_view * m_proj).Invert();
	auto ray_near = DirectX::SimpleMath::Vector3::Transform(point_near, inverse_viewproj);
	auto ray_far = DirectX::SimpleMath::Vector3::Transform(point_far, inverse_viewproj);
	auto ray_subtraction = ray_far - ray_near;
	auto ray_direction = ray_subtraction;
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
