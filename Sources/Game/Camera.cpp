#include "Camera.h"


/// <summary>
/// 視線ベクトルを取得する
/// </summary>
/// <returns>
/// カメラの方向ベクトル
/// </returns>
DirectX::SimpleMath::Vector3 Camera::GetEyeVector() const {
	DirectX::SimpleMath::Vector3 vec = m_target - m_eye;
	vec.Normalize();
	return vec;
}

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
