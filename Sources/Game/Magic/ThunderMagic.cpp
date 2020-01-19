#include "ThunderMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include "MagicFactory.h"
#include "MagicManager.h"
#include <Game\Collision\SphereCollider.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager">���@�}�l�[�W���ւ̃|�C���^</param>
ThunderMagic::ThunderMagic(MagicManager* pMagicManager)
	: Magic()
	, m_pMagicManager(pMagicManager)
	, m_startTimer() 
	, m_time() {
	const MagicParameter::thunder_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderParam;
	m_collider = std::make_unique<SphereCollider>(&m_transform, parameter.colliderRadius);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ThunderMagic::~ThunderMagic() {
}

/// <summary>
/// �����@�𐶐�����
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void ThunderMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::thunder_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	static_cast<SphereCollider*>(m_collider.get())->SetRadius(parameter.colliderRadius);
	m_color = DirectX::Colors::Yellow;
	m_vel = dir * parameter.moveSpeed;
	m_lifeTime = parameter.lifeTime;
	m_startTimer = false;
	m_time = 0.0f;
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

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �����@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ThunderMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::ThunderMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, false);
	//m_sphereCollider.Render(view, proj);
}

/// <summary>
/// �����@�̏I���������s��
/// </summary>
void ThunderMagic::Finalize() {
	// ���ŁE�^�C�}�[�̃J�E���g���B�����痎�����@�𐶐�����
	MagicInfo info = m_info;
	info.id = MagicID::ThunderStrike;
	m_pMagicManager->CreateMagic(info, m_transform.GetLocalPosition(), DirectX::SimpleMath::Vector3::Down);
}

/// <summary>
///  �_���[�W���擾����
/// </summary>
/// <returns>
/// �_���[�W��
/// </returns>
float ThunderMagic::GetPower() const {
	const MagicParameter::thunder_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderParam;

	return parameter.power*m_info.powerRate;
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

