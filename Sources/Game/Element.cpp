#include "Element.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\ElementParameter.h>
#include "PlayParameterLoader.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Element::Element()
	: m_id()
	, m_transform()
	, m_sphereCollider(&m_transform, ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->radius)
	, m_color()
	, m_isUsed(false) {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Element::~Element() {
}

/// <summary>
/// �G�������g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Element::Update(const DX::StepTimer& timer) {
	DirectX::SimpleMath::Quaternion rot = m_transform.GetLocalRotation();
	rot *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY,
		static_cast<float>(timer.GetElapsedSeconds())*Math::HarfPI);
	m_transform.SetRotation(rot);
	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �G�������g���J������
/// </summary>
void Element::Lost() {

}

/// <summary>
/// �G�������g�𐶐�����
/// </summary>
/// <param name="id">�G�������gID</param>
/// <param name="pos">���W</param>
/// <param name="color">�F</param>
void Element::Create(ElementID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector4& color) {
	m_id = id;
	m_transform.SetPosition(pos);
	m_sphereCollider.SetRadius(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->radius);
	m_world = m_transform.GetMatrix();
	m_color = color;
}

/// <summary>
/// �G�������g��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void Element::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::Element);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, false);
}

/// <summary>
/// �t�B�[���h�͈͓̔��Ɏ��߂�
/// </summary>
/// <param name="center">�t�B�[���h�̒��S�_</param>
/// <param name="radius">�t�B�[���h�̔��a</param>
void Element::FitField(const DirectX::SimpleMath::Vector3& center, float radius) {
	constexpr float offset = 1.0f;
	// ���a���l�������͈�
	float limit_range = radius - m_sphereCollider.GetRadius() - 1.0f;
	// �͈͊O�ɂ��邩���肷��
	if (DirectX::SimpleMath::Vector3::DistanceSquared(m_transform.GetPosition(), center) > limit_range*limit_range) {
		// �ʒu�𒲐�����
		DirectX::SimpleMath::Vector3 dir = m_transform.GetPosition() - center;
		dir.Normalize();
		DirectX::SimpleMath::Vector3 pos = center + dir * limit_range;
		m_transform.SetPosition(pos);
	}
}
