#include "Element.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\ElementParameter.h>
#include "PlayParameterLoader.h"


/// <summary>
/// コンストラクタ
/// </summary>
Element::Element()
	: m_id()
	, m_transform()
	, m_sphereCollider(&m_transform, ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->radius)
	, m_color()
	, m_isUsed(false) {
}

/// <summary>
/// デストラクタ
/// </summary>
Element::~Element() {
}

/// <summary>
/// エレメントを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Element::Update(const DX::StepTimer& timer) {
	DirectX::SimpleMath::Quaternion rot = m_transform.GetLocalRotation();
	rot *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY,
		static_cast<float>(timer.GetElapsedSeconds())*Math::HarfPI);
	m_transform.SetRotation(rot);
	m_world = m_transform.GetMatrix();
}

/// <summary>
/// エレメントを開放する
/// </summary>
void Element::Lost() {

}

/// <summary>
/// エレメントを生成する
/// </summary>
/// <param name="id">エレメントID</param>
/// <param name="pos">座標</param>
/// <param name="color">色</param>
void Element::Create(ElementID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector4& color) {
	m_id = id;
	m_transform.SetPosition(pos);
	m_sphereCollider.SetRadius(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->radius);
	m_world = m_transform.GetMatrix();
	m_color = color;
}

/// <summary>
/// エレメントを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void Element::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::Element);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, false);
}

/// <summary>
/// フィールドの範囲内に収める
/// </summary>
/// <param name="center">フィールドの中心点</param>
/// <param name="radius">フィールドの半径</param>
void Element::FitField(const DirectX::SimpleMath::Vector3& center, float radius) {
	constexpr float offset = 1.0f;
	// 半径を考慮した範囲
	float limit_range = radius - m_sphereCollider.GetRadius() - 1.0f;
	// 範囲外にいるか判定する
	if (DirectX::SimpleMath::Vector3::DistanceSquared(m_transform.GetPosition(), center) > limit_range*limit_range) {
		// 位置を調整する
		DirectX::SimpleMath::Vector3 dir = m_transform.GetPosition() - center;
		dir.Normalize();
		DirectX::SimpleMath::Vector3 pos = center + dir * limit_range;
		m_transform.SetPosition(pos);
	}
}
