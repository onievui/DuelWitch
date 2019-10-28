#include "PlayerTrailEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\Color.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Common\Transform.h>


/// <summary>
/// �G�t�F�N�g������������
/// </summary>
/// <param name="lifeTime">���[�v����܂ł̎���</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="accel">�����x</param>
void PlayerTrailEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	Effect::Initialize(lifeTime, pos, vel, accel);
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	HSVColor color(0, parameter.sColor, parameter.vColor);
	color.setH(RandMt::GetRange(parameter.minHColor, parameter.maxHColor));
	m_color = DirectX::SimpleMath::Color(color.getR()*1.0f, color.getG()*1.0f, color.getB()*1.0f, 255.0f) / 255.0f;
	m_defaultSize = RandMt::GetRange(parameter.minScale, parameter.maxScale);
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void PlayerTrailEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	if (m_delayTime > 0.0f) {
		m_delayTime -= elapsed_time;
		if (m_delayTime > 0.0f) {
			return;
		}
		Restart();
	}

	m_vel += m_accel * elapsed_time;
	m_pos += m_vel * elapsed_time;

	m_lifeTime -= elapsed_time;

	// ���Ԃ������Ȃ����烋�[�v����
	if (m_lifeTime < 0) {
		Restart();
	}
}

/// <summary>
/// ���[�v����
/// </summary>
void PlayerTrailEffect::Restart() {
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;

	m_lifeTime = m_startLifeTime;

	// �~����Ƀ����_���ȕ��������߂�
	const float angle = parameter.apexAngle;
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::Up,
		RandMt::GetRange(-angle * 0.5f, angle*0.5f)
	);

	DirectX::SimpleMath::Vector3 center_dir = DirectX::SimpleMath::Vector3::Transform(
		DirectX::SimpleMath::Vector3::UnitZ, m_pParent->GetRotation());
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(center_dir, rotation);
	direction = DirectX::SimpleMath::Vector3::Transform(direction,
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(center_dir, RandMt::GetRand(Math::PI2)));
	direction.Normalize();
	m_pos = m_startPos = m_pParent->GetPosition() + direction * 0.2f;
	m_vel = direction * parameter.speed;
	m_accel = direction * parameter.acceleration;

}

/// <summary>
/// �X�P�[�����擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̃X�P�[��
/// </returns>
float PlayerTrailEffect::GetScale() const {
	return m_defaultSize * (m_lifeTime / m_startLifeTime);
}

/// <summary>
/// �F���擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̐F
/// </returns>
DirectX::SimpleMath::Color PlayerTrailEffect::GetColor() const {
	return m_color;
}
