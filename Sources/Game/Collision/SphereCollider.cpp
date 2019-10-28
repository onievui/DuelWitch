#include "SphereCollider.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Game\Common\Transform.h>


std::unique_ptr<DirectX::GeometricPrimitive> SphereCollider::m_debugSphere = nullptr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pTransform">姿勢へのポインタ</param>
/// <param name="radius">半径</param>
/// <param name="offset">座標のオフセット</param>
SphereCollider::SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset) 
	: Collider(Collider::Type::Sphere, pTransform, offset)
	, m_radius(radius) {
}

/// <summary>
/// 当たり判定を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="color">色</param>
/// <param name="wireframe">ワイヤフレームにするかどうか</param>
void SphereCollider::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Color& color, bool wireframe) const {
	// 実体がない場合は生成する
	if (!m_debugSphere) {
		m_debugSphere = DirectX::GeometricPrimitive::CreateSphere(ServiceLocater<DirectX11>::Get()->GetContext().Get(), 1.0f, 8U);
	}
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::CreateScale(m_radius*2);
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offset);
	matrix *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_pTransform->GetLocalRotation());
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_pTransform->GetLocalPosition());
	m_debugSphere->Draw(matrix, view, proj, color, nullptr, wireframe);
}

