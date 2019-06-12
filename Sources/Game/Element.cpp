#include "Element.h"
#include <Framework/DirectX11.h>

Element::Element()
	: m_object() 
	, m_transform() {
}

Element::~Element() {
}

void Element::Update(const DX::StepTimer& timer) {
	timer;
}

void Element::Lost() {
	m_object.reset();
}

void Element::Create(const DirectX::SimpleMath::Vector3& pos) {
	m_transform.SetPosition(pos);
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), s_radius);
}

void Element::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_transform.GetMatrix(), view, proj, DirectX::Colors::Yellow, nullptr, true);
}
