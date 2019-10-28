#include "SphereCollider.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Game\Common\Transform.h>


std::unique_ptr<DirectX::GeometricPrimitive> SphereCollider::m_debugSphere = nullptr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pTransform">�p���ւ̃|�C���^</param>
/// <param name="radius">���a</param>
/// <param name="offset">���W�̃I�t�Z�b�g</param>
SphereCollider::SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset) 
	: Collider(Collider::Type::Sphere, pTransform, offset)
	, m_radius(radius) {
}

/// <summary>
/// �����蔻���`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="color">�F</param>
/// <param name="wireframe">���C���t���[���ɂ��邩�ǂ���</param>
void SphereCollider::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Color& color, bool wireframe) const {
	// ���̂��Ȃ��ꍇ�͐�������
	if (!m_debugSphere) {
		m_debugSphere = DirectX::GeometricPrimitive::CreateSphere(ServiceLocater<DirectX11>::Get()->GetContext().Get(), 1.0f, 8U);
	}
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::CreateScale(m_radius*2);
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offset);
	matrix *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_pTransform->GetLocalRotation());
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_pTransform->GetLocalPosition());
	m_debugSphere->Draw(matrix, view, proj, color, nullptr, wireframe);
}

