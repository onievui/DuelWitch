#include "Element.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Parameters\ElementParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pBasicEffect">�x�[�V�b�N�G�t�F�N�g�ւ̃|�C���^</param>
/// <param name="pInputLayout">���̓��C�A�E�g�ւ̃|�C���^</param>
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
/// �f�X�g���N�^
/// </summary>
Element::~Element() {
}

/// <summary>
/// �G�������g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Element::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// ��]������
	DirectX::SimpleMath::Quaternion rot = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::UnitY,
		m_time*ELEMENT_ROTATE_SPEED);
	// �o�����͏���������
	float scale = (m_time < 1.0f ? m_time * (2.0f - m_time) : 1.0f);
	m_transform.SetRotation(rot);
	m_transform.SetScale(DirectX::SimpleMath::Vector3::One*scale);
	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �G�������g�𐶐�����
/// </summary>
/// <param name="id">�G�������gID</param>
/// <param name="pos">���W</param>
void Element::Create(ElementID id, const DirectX::SimpleMath::Vector3& pos) {
	m_id = id;
	m_transform.SetPosition(pos);
	m_transform.SetScale(DirectX::SimpleMath::Vector3::Zero);
	m_sphereCollider.SetRadius(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->radius);
	m_world = m_transform.GetMatrix();
	m_time = 0.0f;
}

/// <summary>
/// �G�������g��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void Element::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::Element);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);

	// �p�����[�^���Z�b�g����
	m_pBasicEffect->SetWorld(m_world);
	m_pBasicEffect->SetView(view);
	m_pBasicEffect->SetProjection(proj);
	DirectX::SimpleMath::Color color = DirectX::Colors::AntiqueWhite;
	color.w = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->alphaRate;
	m_pBasicEffect->SetDiffuseColor(color);
	m_pBasicEffect->SetTexture(texture->GetResource(static_cast<int>(m_id)).Get());

	// �e�N�X�`�����������Ȃ��߁A��ɔw�ʂ���`�悷��
	resource->GetResource()->Draw(m_pBasicEffect, m_pInputLayout, true, false, [&]() {
		// �������菈��
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		// ���v���J�����O
		context->RSSetState(states->CullClockwise());
	});
	resource->GetResource()->Draw(m_pBasicEffect, m_pInputLayout, true, false, [&]() {
		// �������菈��
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
		// �����v����J�����O
		context->RSSetState(states->CullCounterClockwise());
	});

}

/// <summary>
/// �t�B�[���h�[�Ƃ̋������L������
/// </summary>
/// <param name="center">�t�B�[���h�̒��S�_</param>
/// <param name="radius">�t�B�[���h�̔��a</param>
void Element::SetBetweenFieldRadius(const DirectX::SimpleMath::Vector3& center, float radius) {
	m_betweenFieldEnd = radius - DirectX::SimpleMath::Vector3::Distance(center, m_transform.GetPosition());
}

/// <summary>
/// �t�B�[���h�Ƃ̈ʒu�֌W��ۂ�
/// </summary>
/// <param name="center">�t�B�[���h�̒��S�_</param>
/// <param name="radius">�t�B�[���h�̔��a</param>
void Element::FitField(const DirectX::SimpleMath::Vector3& center, float radius) {
	// ����ׂ����S����̋���
	float limit_range = radius - m_betweenFieldEnd;
	// �ʒu�𒲐�����
	DirectX::SimpleMath::Vector3 dir = m_transform.GetPosition() - center;
	dir.Normalize();
	DirectX::SimpleMath::Vector3 pos = center + dir * limit_range;
	m_transform.SetPosition(pos);
}
