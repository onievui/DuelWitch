#include "Element.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Parameters\ElementParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pBasicEffect">ベーシックエフェクトへのポインタ</param>
/// <param name="pInputLayout">入力レイアウトへのポインタ</param>
Element::Element(DirectX::BasicEffect* pBasicEffect, ID3D11InputLayout* pInputLayout)
	: m_id()
	, m_transform()
	, m_sphereCollider(&m_transform, ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->radius)
	, m_pBasicEffect(pBasicEffect)
	, m_pInputLayout(pInputLayout)
	, m_isUsed(false)
	, m_time()
	, m_betweenFieldEnd() {
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
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// 回転させる
	DirectX::SimpleMath::Quaternion rot = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::UnitY,
		m_time*ELEMENT_ROTATE_SPEED);
	// 出現時は小さくする
	float scale = (m_time < 1.0f ? m_time * (2.0f - m_time) : 1.0f);
	m_transform.SetRotation(rot);
	m_transform.SetScale(DirectX::SimpleMath::Vector3::One*scale);
	m_world = m_transform.GetMatrix();
}

/// <summary>
/// エレメントを生成する
/// </summary>
/// <param name="id">エレメントID</param>
/// <param name="pos">座標</param>
void Element::Create(ElementID id, const DirectX::SimpleMath::Vector3& pos) {
	m_id = id;
	m_transform.SetPosition(pos);
	m_transform.SetScale(DirectX::SimpleMath::Vector3::Zero);
	m_sphereCollider.SetRadius(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->radius);
	m_world = m_transform.GetMatrix();
	m_time = 0.0f;
}

/// <summary>
/// エレメントを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void Element::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::Element);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);

	// パラメータをセットする
	m_pBasicEffect->SetWorld(m_world);
	m_pBasicEffect->SetView(view);
	m_pBasicEffect->SetProjection(proj);
	DirectX::SimpleMath::Color color = DirectX::Colors::AntiqueWhite;
	color.w = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->alphaRate;
	m_pBasicEffect->SetDiffuseColor(color);
	m_pBasicEffect->SetTexture(texture->GetResource(static_cast<int>(m_id)).Get());

	// テクスチャが半透明なため、先に背面から描画する
	resource->GetResource()->Draw(m_pBasicEffect, m_pInputLayout, true, false, [&]() {
		// 透明判定処理
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		// 時計回りカリング
		context->RSSetState(states->CullClockwise());
	});
	resource->GetResource()->Draw(m_pBasicEffect, m_pInputLayout, true, false, [&]() {
		// 透明判定処理
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		// 反時計周りカリング
		context->RSSetState(states->CullCounterClockwise());
	});

}

/// <summary>
/// フィールド端との距離を記憶する
/// </summary>
/// <param name="center">フィールドの中心点</param>
/// <param name="radius">フィールドの半径</param>
void Element::SetBetweenFieldRadius(const DirectX::SimpleMath::Vector3& center, float radius) {
	m_betweenFieldEnd = radius - DirectX::SimpleMath::Vector3::Distance(center, m_transform.GetPosition());
}

/// <summary>
/// フィールドとの位置関係を保つ
/// </summary>
/// <param name="center">フィールドの中心点</param>
/// <param name="radius">フィールドの半径</param>
void Element::FitField(const DirectX::SimpleMath::Vector3& center, float radius) {
	// いるべき中心からの距離
	float limit_range = radius - m_betweenFieldEnd;
	// 位置を調整する
	DirectX::SimpleMath::Vector3 dir = m_transform.GetPosition() - center;
	dir.Normalize();
	DirectX::SimpleMath::Vector3 pos = center + dir * limit_range;
	m_transform.SetPosition(pos);
}
