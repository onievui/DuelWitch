#include "ThunderMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "MagicFactory.h"
#include "MagicManager.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager">���@�}�l�[�W��</param>
ThunderMagic::ThunderMagic(MagicManager* magicManager)
	: Magic(MagicID::Thunder)
	, m_pMagicManager(magicManager)
	, m_startTimer() 
	, m_time() {
	m_sphereCollider.SetRadius(ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderParam.colliderRadius);
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
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	// ���������p�^�C�}�[�̃J�E���g
	if (m_startTimer) {
		m_time += elapsed_time;
		if (m_time > parameter->thunderParam.chaseEndTime) {
			m_isUsed = false;
		}
	}
	
	DirectX::SimpleMath::Vector3 pos = m_transform.GetLocalPosition();
	pos += m_vel*elapsed_time;	
	m_transform.SetPosition(pos);

	// ���ŁE�^�C�}�[�̃J�E���g���B�����痎�����@�𐶐�����
	if (!m_isUsed) {
		m_pMagicManager->CreateMagic(MagicID::ThunderStrike, m_playerId, pos, DirectX::SimpleMath::Vector3::Down);
	}

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �����@���J������
/// </summary>
void ThunderMagic::Lost() {

}

/// <summary>
/// �����@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
/// <param name="color">�F</param>
void ThunderMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
	const DirectX::SimpleMath::Vector4& color) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_sphereCollider.SetRadius(parameter->thunderParam.colliderRadius);
	m_color = color;
	m_vel = dir*parameter->thunderParam.moveSpeed;
	m_lifeTime = parameter->thunderParam.lifeTime;
	m_startTimer = false;
	m_time = 0.0f;
}

/// <summary>
/// �����@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ThunderMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::ThunderMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj);
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void ThunderMagic::HitPlayer(const Collider* collider) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	DirectX::SimpleMath::Vector3 direction = collider->GetTransform()->GetLocalPosition() - m_transform.GetLocalPosition();
	// ���Ε����Ȃ珈�����Ȃ�
	if (direction.z*m_vel.z < 0) {
		return;
	}
	// ��苗���܂ŋ߂Â����瑦������
	if (DirectX::SimpleMath::Vector2(direction.x, direction.z).Length() < parameter->thunderParam.chaseEndDistance) {
		m_isUsed = false;
	}
	direction.Normalize();
	m_vel = Math::Lerp(m_vel, direction * parameter->thunderParam.chaseSpeed, parameter->thunderParam.chaseLerpSpeed);
	// �G�v���C���[�ɐڋ߂�����^�C�}�[�쓮
	m_startTimer = true;
}

/// <summary>
/// ���@�Ƃ̏Փˏ���
/// </summary>
/// <param name="other">�Փ˂������@</param>
void ThunderMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	other_id;
}

