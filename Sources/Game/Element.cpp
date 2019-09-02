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
	DirectX::SimpleMath::Quaternion rot = m_transform.GetRotation();
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
/// �G�������g�̍s����擾����
/// </summary>
/// <returns>
/// �s��
/// </returns>
const DirectX::SimpleMath::Matrix& Element::GetMatrix() const {
	return m_world;
}

/// <summary>
/// �G�������g�̓����蔻����擾����
/// </summary>
/// <returns>
/// �����蔻��
/// </returns>
const SphereCollider* Element::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// �G�������g��ID���擾����
/// </summary>
/// <returns>
/// �G�������gID
/// </returns>
ElementID Element::GetID() const {
	return m_id;
}

/// <summary>
/// �G�������g���g�p���Ă��邩�ǂ����擾����
/// </summary>
/// <returns>
/// true : �g�p���Ă���
/// false : �g�p���Ă��Ȃ�
/// </returns>
bool Element::IsUsed() const {
	return m_isUsed;
}

/// <summary>
/// �G�������g���g�p���邩�ǂ����ݒ肷��
/// </summary>
/// <param name="isUsed">true : �g�p����, false : �g�p���Ȃ�</param>
void Element::IsUsed(bool isUsed) {
	m_isUsed = isUsed;
}

