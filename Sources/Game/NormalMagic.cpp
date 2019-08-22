#include "NormalMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include "MagicFactory.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
NormalMagic::NormalMagic()
	: Magic(MagicID::Normal) {
	m_sphereCollider.SetRadius(NORMAL_MAGIC_RADIUS);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
NormalMagic::~NormalMagic() {
}

/// <summary>
/// �ʏ햂�@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void NormalMagic::Update(const DX::StepTimer& timer) {
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
/// �ʏ햂�@���J������
/// </summary>
void NormalMagic::Lost() {

}

/// <summary>
/// �ʏ햂�@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="color">�F</param>
void NormalMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_lifeTime = 10.0f;
}

/// <summary>
/// �ʏ햂�@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void NormalMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::NormalMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void NormalMagic::HitPlayer(const SphereCollider& collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// ���@�Ƃ̏Փˏ���
/// </summary>
/// <param name="other">�Փ˂������@</param>
void NormalMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// �X���@�ƏՓ˂����������
	if (other_id == MagicID::Freeze) {
		m_isUsed = false;
	}
}
