#include "ThunderMagic.h"
#include <Framework/DirectX11.h>
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ThunderMagic::ThunderMagic()
	: Magic()
	, m_object() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ThunderMagic::~ThunderMagic() {
}

/// <summary>
/// �����@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void ThunderMagic::Update(const DX::StepTimer& timer) {
	m_lifeTime -= float(timer.GetElapsedSeconds());
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetPosition();
	pos += m_vel;	
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �����@���J������
/// </summary>
void ThunderMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// �����@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="color">�F</param>
void ThunderMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), THUNDER_MAGIC_RADIUS);
	m_lifeTime = 5.0f;
}

/// <summary>
/// �����@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ThunderMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void ThunderMagic::HitPlayer(const SphereCollider& collider) {
	auto direction = collider.GetTransform()->GetPosition() - m_transform.GetPosition();
	direction.Normalize();
	m_vel = Math::Lerp(m_vel, direction * THUNDER_MAGIC_CHASE_SPEED, 0.5f);
}
