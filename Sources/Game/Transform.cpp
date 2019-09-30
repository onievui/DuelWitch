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

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pParent">親オブジェクトへのポインタ</param>
/// <param name="position">座標</param>
/// <param name="rotation">回転</param>
/// <param name="scale">スケール</param>
ChildTransform::ChildTransform(const Transform* pParent, const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation, const DirectX::SimpleMath::Vector3& scale)
	: Transform(position, rotation, scale)
	, m_pParent(pParent) {
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pParent">親オブジェクトへのポインタ</param>
/// <param name="position">座標</param>
/// <param name="euler">オイラー角</param>
/// <param name="scale">スケール</param>
ChildTransform::ChildTransform(const Transform* pParent, const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& euler, const DirectX::SimpleMath::Vector3& scale)
	: Transform(position, euler, scale)
	, m_pParent(pParent) {
}

/// <summary>
/// 座標を取得する
/// </summary>
/// <returns>
/// 座標
/// </returns>
const DirectX::SimpleMath::Vector3 ChildTransform::GetPosition() const {
	if (m_pParent) {
		return DirectX::SimpleMath::Vector3::Transform(m_position, m_pParent->GetMatrix());
	}
	return GetLocalPosition();
}

/// <summary>
/// 回転を取得する
/// </summary>
/// <returns>
/// 回転
/// </returns>
const DirectX::SimpleMath::Quaternion ChildTransform::GetRotation() const {
	if (m_pParent) {
		return m_rotation * m_pParent->GetRotation();
	}
	return GetLocalRotation();
}

/// <summary>
/// スケールを取得する
/// </summary>
/// <returns>
/// スケール
/// </returns>
const DirectX::SimpleMath::Vector3 ChildTransform::GetScale() const {
	if (m_pParent) {
		return m_scale * m_pParent->GetScale();
	}
	return GetLocalScale();
}

/// <summary>
/// ワールド行列を取得する
/// </summary>
/// <returns>
/// ワールド行列
/// </returns>
DirectX::SimpleMath::Matrix ChildTransform::GetMatrix() const {
	if (m_pParent) {
		return Transform::GetMatrix()*m_pParent->GetMatrix();
	}
	return Transform::GetMatrix();
}

/// <summary>
/// 親オブジェクトを設定する
/// </summary>
/// <param name="pParent">親オブジェクトへのポインタ</param>
void ChildTransform::SetParent(const Transform* pParent) {
	m_pParent = pParent;
}
