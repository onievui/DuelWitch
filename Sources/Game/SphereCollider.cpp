#include "SphereCollider.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include "Transform.h"


std::unique_ptr<DirectX::GeometricPrimitive> SphereCollider::m_debugSphere = nullptr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pTransform">姿勢クラスへのポインタ</param>
/// <param name="radius">半径</param>
/// <param name="offset">座標のオフセット</param>
SphereCollider::SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset) 
	: Collider(Collider::Type::Sphere, pTransform, offset)
	, m_radius(radius) {
}

/// <summary>
/// 当たり判定の表示
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="color">色</param>
/// <param name="wireframe">ワイヤフレームにするかどうか</param>
void SphereCollider::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Color& color, bool wireframe) const {
	// 実体がない場合は生成する
	if (!m_debugSphere) {
		m_debugSphere = DirectX::GeometricPrimitive::CreateSphere(ServiceLocater<DirectX11>::Get()->GetContext().Get());
	}
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::CreateScale(m_radius*2);
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offset);
	matrix *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_pTransform->GetRotation());
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_pTransform->GetPosition());
	m_debugSphere->Draw(matrix, view, proj, color, nullptr, wireframe);
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

