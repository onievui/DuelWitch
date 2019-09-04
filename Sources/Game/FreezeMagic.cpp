#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
FreezeMagic::FreezeMagic()
	: Magic(MagicID::Freeze)
	, m_pPlayerPos() {
	m_sphereCollider.SetRadius(ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam.radius);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FreezeMagic::~FreezeMagic() {
}

/// <summary>
/// �X���@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FreezeMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = *m_pPlayerPos;
	//pos += m_vel;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �X���@���J������
/// </summary>
void FreezeMagic::Lost() {

}

/// <summary>
/// �X���@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
/// <param name="color">�F</param>
void FreezeMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	m_sphereCollider.SetRadius(parameter->freezeParam.radius);
	m_pPlayerPos = &pos;
	m_color = color;
	m_vel = dir;
	m_lifeTime = parameter->freezeParam.lifeTime;
}

/// <summary>
/// �X���@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void FreezeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::FreezeMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj);
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void FreezeMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// ���@�Ƃ̏Փˏ���
/// </summary>
/// <param name="other">�Փ˂������@</param>
void FreezeMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// �����@�ƏՓ˂����������
	if (other_id == MagicID::Fire) {
		m_isUsed = false;
	}
}
