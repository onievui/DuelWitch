#include "Camera.h"


/// <summary>
/// スクリーン座標からレイに変換する
/// </summary>
/// <param name="point">スクリーン座標</param>
/// <returns>
/// レイ
/// </returns>
DirectX::SimpleMath::Ray Camera::ScreenPointToRay(const DirectX::SimpleMath::Vector3& point) const {
	return ViewportPointToRay(ScreenToViewportPoint(point));
}

/// <summary>
/// ビューポート座標からレイに変換する
/// </summary>
/// <param name="point">ビューポート座標</param>
/// <returns>
/// レイ
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
/// ワールド座標からスクリーン座標に変換する
/// </summary>
/// <param name="point">ワールド座標</param>
/// <returns>
/// スクリーン座標
/// </returns>
DirectX::SimpleMath::Vector3 Camera::WorldToScreenPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_view * m_proj * m_viewport);
}

/// <summary>
/// ワールド座標からビューポート座標に変換する
/// </summary>
/// <param name="point">ワールド座標</param>
/// <returns>
/// ビューポート座標
/// </returns>
DirectX::SimpleMath::Vector3 Camera::WorldToViewportPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_view * m_proj);
}

/// <summary>
/// ビューポート座標からワールド座標に変換する
/// </summary>
/// <param name="point">ビューポート座標</param>
/// <returns>
/// ワールド座標
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ViewportToWorldPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, (m_view* m_proj).Invert());
}

/// <summary>
/// スクリーン座標からワールド座標に変換する
/// </summary>
/// <param name="point">スクリーン座標</param>
/// <returns>
/// ワールド座標
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ScreenToWorldPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, (m_view* m_proj * m_viewport).Invert());
}

/// <summary>
/// スクリーン座標からビューポート座標に変換する
/// </summary>
/// <param name="point">スクリーン座標</param>
/// <returns>
/// ビューポート座標
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ScreenToViewportPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_viewport.Invert());
}

/// <summary>
/// ビューポート座標からスクリーン座標に変換する
/// </summary>
/// <param name="point">ビューポート座標</param>
/// <returns>
/// スクリーン座標
/// </returns>
DirectX::SimpleMath::Vector3 Camera::ViewportToScreenPoint(const DirectX::SimpleMath::Vector3& point) const {
	return DirectX::SimpleMath::Vector3::Transform(point, m_viewport);
}
