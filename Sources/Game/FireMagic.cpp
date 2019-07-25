#include "FireMagic.h"
#include <Framework/DirectX11.h>
#include "MagicFactory.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
FireMagic::FireMagic()
	: Magic(MagicID::Fire)
	, m_object() {
	m_sphereCollider.SetRadius(FIRE_MAGIC_RADIUS);
	m_sphereCollider.SetOffset(DirectX::SimpleMath::Vector3(0, -FIRE_MAGIC_RADIUS/2, 0));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FireMagic::~FireMagic() {
}

/// <summary>
/// �����@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FireMagic::Update(const DX::StepTimer& timer) {
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
void FireMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// �����@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="color">�F</param>
void FireMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);

	// �����x�N�g�������ɉ~���̉�]�p�x�����߂�
	float rot_x = 0;
	if (vel.z > 0) {
		rot_x = acos(vel.y);
	}
	else {
		rot_x = -acos(vel.y);
	}
	float rot_z = 0;
	if (vel.x > 0) {
		rot_z = -acos(1 - vel.x);
	}
	else {
		rot_z = acos(1 + vel.x);
	}
	m_transform.SetRotation(DirectX::SimpleMath::Vector3(rot_x, 0, rot_z));

	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), FIRE_MAGIC_RADIUS);
	m_object = DirectX::GeometricPrimitive::CreateCone(DirectX11::Get().GetContext().Get(), FIRE_MAGIC_RADIUS, FIRE_MAGIC_RADIUS*2);
	m_lifeTime = 8.0f;
}

/// <summary>
/// �����@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void FireMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
	m_sphereCollider.Render(view, proj);
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void FireMagic::HitPlayer(const SphereCollider& collider) {
	collider;
	m_isUsed = false;
}
