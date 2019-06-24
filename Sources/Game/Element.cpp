#include "Element.h"
#include <Framework/DirectX11.h>

/// <summary>
/// �R���X�g���N�^
/// </summary>
Element::Element()
	: m_object()
	, m_transform()
	, m_sphereCollider(&m_transform, ELEMENT_RADIUS)
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
	timer;
	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �G�������g���J������
/// </summary>
void Element::Lost() {
	m_object.reset();
}

/// <summary>
/// �G�������g�𐶐�����
/// </summary>
/// <param name="pos">���W</param>
/// <param name="color">�F</param>
void Element::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector4& color) {
	m_transform.SetPosition(pos);
	m_color = color;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), ELEMENT_RADIUS);
}

/// <summary>
/// �G�������g��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void Element::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
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
