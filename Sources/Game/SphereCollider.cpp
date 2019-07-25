#include "SphereCollider.h"
#include <Framework\DirectX11.h>
#include "Transform.h"


std::unique_ptr<DirectX::GeometricPrimitive> SphereCollider::m_debugSphere = nullptr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pTransform">�p���N���X�ւ̃|�C���^</param>
/// <param name="radius">���a</param>
/// <param name="offset">���W�̃I�t�Z�b�g</param>
SphereCollider::SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset) 
	: m_pTransform(pTransform)
	, m_radius(radius)
	, m_offset(offset) {
}

/// <summary>
/// �Փ˔���
/// </summary>
/// <param name="other">���I�u�W�F�N�g�̓����蔻��</param>
/// <returns>
/// true : �Փ˂��Ă���
/// fase : �Փ˂��Ă��Ȃ�
/// </returns>
bool SphereCollider::Collision(const SphereCollider* other) const {
	auto& rot = m_pTransform->GetRotation();
	auto offset = DirectX::SimpleMath::Vector3::Transform(m_offset,
		DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rot.y, rot.z, rot.z));
	auto& rot2 = other->m_pTransform->GetRotation();
	auto offset2 = DirectX::SimpleMath::Vector3::Transform(other->m_offset,
		DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rot2.y, rot2.z, rot2.z));
	DirectX::SimpleMath::Vector3 d = (m_pTransform->GetPosition() + offset) - (other->m_pTransform->GetPosition() + offset2);
	float dist2 = d.x*d.x + d.y*d.y + d.z*d.z;
	return dist2 <= (m_radius + other->m_radius)*(m_radius + other->m_radius);
}

/// <summary>
/// �����蔻��̕\��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="color">�F</param>
void SphereCollider::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Vector4& color) const {
	// ���̂��Ȃ��ꍇ�͐�������
	if (!m_debugSphere) {
		m_debugSphere = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get());
	}
	auto matrix = DirectX::SimpleMath::Matrix::CreateScale(m_radius);
	auto& rot = m_pTransform->GetRotation();
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_offset);
	matrix *= DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	matrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_pTransform->GetPosition());
	m_debugSphere->Draw(matrix, view, proj, color, nullptr, true);
}

/// <summary>
/// �p���N���X�ւ̃|�C���^���擾����
/// </summary>
/// <returns>
/// �p���N���X�ւ̃|�C���^
/// </returns>
const Transform* SphereCollider::GetTransform() const {
	return m_pTransform;
}

/// <summary>
/// �p���N���X�ւ̃|�C���^��ݒ肷��
/// </summary>
/// <param name="pTransform">�p���N���X�ւ̃|�C���^</param>
void SphereCollider::SetTransform(const Transform* pTransform) {
	m_pTransform = pTransform;
}

/// <summary>
/// ���a���擾����
/// </summary>
/// <returns>
/// ���a
/// </returns>
float SphereCollider::GetRadius() const {
	return m_radius;
}

/// <summary>
/// ���a��ݒ肷��
/// </summary>
/// <param name="radius">���a</param>
void SphereCollider::SetRadius(float radius) {
	m_radius = radius;
}

/// <summary>
/// �I�t�Z�b�g���擾����
/// </summary>
/// <returns>
/// �I�t�Z�b�g
/// </returns>
const DirectX::SimpleMath::Vector3& SphereCollider::GetOffset() const {
	return m_offset;
}

/// <summary>
/// �I�t�Z�b�g��ݒ肷��
/// </summary>
/// <param name="offset">�I�t�Z�b�g</param>
void SphereCollider::SetOffset(const DirectX::SimpleMath::Vector3& offset) {
	m_offset = offset;
}
