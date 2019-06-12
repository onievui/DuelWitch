#include "Transform.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="rotation">回転</param>
/// <param name="scale">スケール</param>
Transform::Transform(const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& rotation,
	const DirectX::SimpleMath::Vector3& scale)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(scale) {

}

/// <summary>
/// 座標を取得する
/// </summary>
/// <returns>
/// 座標
/// </returns>
const DirectX::SimpleMath::Vector3& Transform::GetPosition() const {
	return m_position;
}

/// <summary>
/// 座標を設定する
/// </summary>
/// <param name="pos">座標</param>
void Transform::SetPosition(const DirectX::SimpleMath::Vector3& position) {
	m_position = position;
}

/// <summary>
/// 回転を取得する
/// </summary>
/// <returns>
/// 回転
/// </returns>
const DirectX::SimpleMath::Vector3& Transform::GetRotation() const {
	return m_rotation;
}

/// <summary>
/// 回転を設定する
/// </summary>
/// <param name="rotation">回転</param>
void Transform::SetRotation(const DirectX::SimpleMath::Vector3& rotation) {
	m_rotation = rotation;
}

/// <summary>
/// スケールを取得する
/// </summary>
/// <returns>
/// スケール
/// </returns>
const DirectX::SimpleMath::Vector3& Transform::GetScale() const {
	return m_scale;
}

/// <summary>
/// スケールを設定する
/// </summary>
/// <param name="scale">スケール</param>
void Transform::SetScale(const DirectX::SimpleMath::Vector3& scale) {
	m_scale = scale;
}

/// <summary>
/// ワールド行列を取得する
/// </summary>
/// <returns>
/// ワールド行列
/// </returns>
DirectX::SimpleMath::Matrix Transform::GetMatrix() const {
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	DirectX::SimpleMath::Quaternion quaternion = 
		DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
	world *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	return world;
}
