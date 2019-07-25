#include "Element.h"
#include <Framework/DirectX11.h>

/// <summary>
/// コンストラクタ
/// </summary>
Element::Element()
	: m_object()
	, m_id()
	, m_transform()
	, m_sphereCollider(&m_transform, ELEMENT_RADIUS)
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
	timer;
	auto rot = m_transform.GetRotation();
	rot.y += float(timer.GetElapsedSeconds())*Math::HarfPI;
	m_transform.SetRotation(rot);
	m_world = m_transform.GetMatrix();
}

/// <summary>
/// エレメントを開放する
/// </summary>
void Element::Lost() {
	m_object.reset();
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
	m_world = m_transform.GetMatrix();
	m_color = color;
	m_object = DirectX::GeometricPrimitive::CreateCube(DirectX11::Get().GetContext().Get(), ELEMENT_RADIUS*2);
}

/// <summary>
/// エレメントを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void Element::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, false);
}

/// <summary>
/// エレメントの行列を取得する
/// </summary>
/// <returns>
/// 行列
/// </returns>
const DirectX::SimpleMath::Matrix& Element::GetMatrix() const {
	return m_world;
}

/// <summary>
/// エレメントの当たり判定を取得する
/// </summary>
/// <returns>
/// 当たり判定
/// </returns>
const SphereCollider* Element::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// エレメントのIDを取得する
/// </summary>
/// <returns>
/// エレメントID
/// </returns>
ElementID Element::GetID() const {
	return m_id;
}

/// <summary>
/// エレメントを使用しているかどうか取得する
/// </summary>
/// <returns>
/// true : 使用している
/// false : 使用していない
/// </returns>
bool Element::IsUsed() const {
	return m_isUsed;
}

/// <summary>
/// エレメントを使用するかどうか設定する
/// </summary>
/// <param name="isUsed">true : 使用する, false : 使用しない</param>
void Element::IsUsed(bool isUsed) {
	m_isUsed = isUsed;
}
