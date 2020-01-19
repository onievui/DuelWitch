#include "DeathEffect.h"
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
void DeathEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	const EffectParameter::death_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->deathParam;

	Effect::Initialize(lifeTime, pos, vel, accel);

	m_startScale = RandMt::GetRange(parameter.minScale, parameter.maxScale);
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer"></param>
void DeathEffect::Update(const DX::StepTimer& timer) {
	Effect::Update(timer);
}

/// <summary>
/// �G�t�F�N�g�̃X�P�[�����擾����
/// </summary>
/// <returns>
/// �X�P�[��
/// </returns>
float DeathEffect::GetScale() const {
	const EffectParameter::death_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->deathParam;

	float t = std::sinf(Math::PI2*(m_startLifeTime - m_lifeTime)*parameter.blinkSpeed);
	return (t*0.5f + 1.0f)*m_startScale;
}

/// <summary>
/// �G�t�F�N�g�̐F�̋������擾����
/// </summary>
/// <returns>
/// �F�̋���
/// </returns>
float DeathEffect::GetColorPower() const {
	const EffectParameter::death_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->deathParam;

	float t = std::sinf(Math::PI2*(m_startLifeTime - m_lifeTime)*parameter.blinkSpeed);
	return (t + 1.0f)*0.5f*0.7f + 0.3f;
}
