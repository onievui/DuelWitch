#include "NormalMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include <Game\Collision\SphereCollider.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\IEffectEmitter.h>
#include <Game\Player\PlayerData.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
NormalMagic::NormalMagic()
	: Magic() {
	const MagicParameter::normal_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->normalParam;
	m_collider = std::make_unique<SphereCollider>(&m_transform, parameter.radius);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
NormalMagic::~NormalMagic() {
}

/// <summary>
/// �ʏ햂�@�𐶐�����
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void NormalMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::normal_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->normalParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	static_cast<SphereCollider*>(m_collider.get())->SetRadius(parameter.radius);
	m_color = DirectX::Colors::White;
	m_vel = dir * parameter.moveSpeed;
	m_lockOnTimer = parameter.lockOnTime;
	m_lifeTime = parameter.lifeTime;

	// ���@�̃G�t�F�N�g�𐶐�����
	m_pEffect = ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::NormalMagic, pos, dir);
	m_pEffect->SetParent(&m_transform);
}

/// <summary>
/// �ʏ햂�@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void NormalMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetLocalPosition();
	// ���b�N�I�����͒ǔ�������
	if (m_info.lockOnPlayerId != -1 && m_lockOnTimer > 0.0f) {
		m_lockOnTimer -= elapsed_time;
		DirectX::SimpleMath::Vector3 target_pos = ServiceLocater<PlayerData>::Get()->transforms[m_info.lockOnPlayerId]->GetPosition();
		DirectX::SimpleMath::Vector3 target_dir = target_pos - pos;
		// 90���ȏ�Ȃ�ǔ����I��������
		if (m_vel.Dot(target_dir) >= 0.0f) {
			float angle = Math::BetweenAngle(m_vel, target_dir);
			const float rotate_speed = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam.lockOnRotateSpeed;
			m_vel = DirectX::SimpleMath::Vector3::Transform(
				m_vel, DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(m_vel.Cross(target_dir), std::min(angle, rotate_speed*elapsed_time)));
		}
		else {
			m_lockOnTimer = 0.0f;
		}
	}
	pos += m_vel*elapsed_time;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �ʏ햂�@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void NormalMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::NormalMagic);
	resource->GetResource()->Draw(m_world, view, proj, DirectX::SimpleMath::Color(0, 0.5f, 1, 0.5f), nullptr, false);
}

/// <summary>
/// �ʏ햂�@�̏I���������s��
/// </summary>
void NormalMagic::Finalize() {
	// �G�t�F�N�g���I��������
	m_pEffect->SetUsed(false);
}

/// <summary>
///  �_���[�W���擾����
/// </summary>
/// <returns>
/// �_���[�W��
/// </returns>
float NormalMagic::GetPower() const {
	const MagicParameter::normal_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->normalParam;

	return parameter.power*m_info.powerRate;
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void NormalMagic::HitPlayer(const Collider* collider) {
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
