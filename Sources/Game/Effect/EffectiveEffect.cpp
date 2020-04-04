#include "EffectiveEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\RandMt.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// �G�t�F�N�g������������
/// </summary>
/// <param name="lifeTime">���[�v����܂ł̎���</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="accel">�����x</param>
void EffectiveEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	const EffectParameter::effective_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->effectiveParam;

	Effect::Initialize(lifeTime, pos, vel, accel);

	m_angle = RandMt::GetRand(Math::PI2);
	m_rotateSpeed = RandMt::GetRange(parameter.minRotateSpeed, parameter.maxRotateSpeed)*Math::PI2*(RandMt::GetRand(2)*2-1);
	m_scale = RandMt::GetRange(parameter.minScale, parameter.maxScale);
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void EffectiveEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// ���X�ɒx������
	float t = m_lifeTime / m_startLifeTime;
	t = t * (2 - t);
	m_vel = m_startVel * t;
	m_pos += m_vel * elapsed_time;
	m_angle += m_rotateSpeed*elapsed_time;

	m_lifeTime -= elapsed_time;
}

