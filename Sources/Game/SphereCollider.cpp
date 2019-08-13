#include "SphereCollider.h"
#include <Framework\DirectX11.h>
#include "Transform.h"


std::unique_ptr<DirectX::GeometricPrimitive> SphereCollider::m_debugSphere = nullptr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pTransform">姿勢クラスへのポインタ</param>
/// <param name="radius">半径</param>
/// <param name="offset">座標のオフセット</param>
SphereCollider::SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset) 
	: Collider(Collider::Type::Sphere)
	, m_pTransform(pTransform)
	, m_radius(radius)
	, m_offset(offset) {
}

/// <summary>
/// 当たり判定の表示
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="color">色</param>
void SphereCollider::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Vector4& color) const {
	// 実体がない場合は生成する
	if (!m_debugSphere) {
		m_debugSphere = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get());
	}
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::CreateScale(m_radius);
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offset);
	matrix *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_pTransform->GetRotation());
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_pTransform->GetPosition());
	m_debugSphere->Draw(matrix, view, proj, color, nullptr, true);
}

/// <summary>
/// 姿勢クラスへのポインタを取得する
/// </summary>
/// <returns>
/// 姿勢クラスへのポインタ
/// </returns>
const Transform* SphereCollider::GetTransform() const {
	return m_pTransform;
}

/// <summary>
/// 姿勢クラスへのポインタを設定する
/// </summary>
/// <param name="pTransform">姿勢クラスへのポインタ</param>
void SphereCollider::SetTransform(const Transform* pTransform) {
	m_pTransform = pTransform;
}

/// <summary>
/// 半径を取得する
/// </summary>
/// <returns>
/// 半径
/// </returns>
float SphereCollider::GetRadius() const {
	return m_radius;
}

/// <summary>
/// 半径を設定する
/// </summary>
/// <param name="radius">半径</param>
void SphereCollider::SetRadius(float radius) {
	m_radius = radius;
}

/// <summary>
/// オフセットを取得する
/// </summary>
/// <returns>
/// オフセット
/// </returns>
const DirectX::SimpleMath::Vector3& SphereCollider::GetOffset() const {
	return m_offset;
}

/// <summary>
/// オフセットを設定する
/// </summary>
/// <param name="offset">オフセット</param>
void SphereCollider::SetOffset(const DirectX::SimpleMath::Vector3& offset) {
	m_offset = offset;
}
