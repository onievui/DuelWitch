#include "Transform.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="rotation">回転</param>
/// <param name="scale">スケール</param>
Transform::Transform(const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(scale) {
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="euler">オイラー角</param>
/// <param name="scale">スケール</param>
Transform::Transform(const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& euler,
	const DirectX::SimpleMath::Vector3& scale) 
	: m_position(position)
	, m_rotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z))
	, m_scale(scale) {
}

/// /// <summary>
/// ワールド行列を取得する
/// </summary>
/// <returns>
/// ワールド行列
/// </returns>
DirectX::SimpleMath::Matrix Transform::GetMatrix() const {
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	world *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	return world;
}
