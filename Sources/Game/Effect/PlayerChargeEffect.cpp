#include "PlayerChargeEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\Color.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// �G�t�F�N�g������������
/// </summary>
/// <param name="lifeTime">���[�v����܂ł̎���</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="accel">�����x</param>
void PlayerChargeEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	Effect::Initialize(lifeTime, pos, vel, accel);
	m_delayTime = 0.0f;
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void PlayerChargeEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	if (m_delayTime > 0.0f) {
		m_delayTime -= elapsed_time;
		if (m_delayTime > 0.0f) {
			return;
		}
		Restart();
	}

	m_lifeTime -= elapsed_time;

	// ���Ԃ������Ȃ����烋�[�v����
	if (m_lifeTime < 0) {
		Restart();
	}
}

/// <summary>
/// ���[�v����
/// </summary>
void PlayerChargeEffect::Restart() {
	m_lifeTime = m_startLifeTime;
}

/// <summary>
/// �X�P�[�����擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̃X�P�[��
/// </returns>
float PlayerChargeEffect::GetScale() const {
	const EffectParameter::player_charge_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	return Math::Lerp(parameter.minScale, parameter.maxScale, m_lifeTime / m_startLifeTime);
}


