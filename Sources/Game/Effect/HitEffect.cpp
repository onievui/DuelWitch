#include "HitEffect.h"
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
void HitEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	Effect::Initialize(lifeTime, pos, vel, accel);
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer"></param>
void HitEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_lifeTime -= elapsed_time;

}

/// <summary>
/// �X�P�[�����擾����
/// </summary>
/// <returns>
/// �p�[�e�B�N���̃X�P�[��
/// </returns>
float HitEffect::GetScale() const {
	EffectParameter::hit_param parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->hitParam;

	// �ŏ�����2����1�̎��Ԃōő�X�P�[���܂ŕω�������
	if (m_lifeTime / m_startLifeTime > 0.5f) {
		float t = (m_startLifeTime - m_lifeTime) / m_startLifeTime / (1 - 0.5f);
		t = t * (2 - t);
		return parameter.scale * t;
	}

	return parameter.scale;
}

/// <summary>
/// �A���t�@���擾����
/// </summary>
/// <returns>
/// �p�[�e�B�N���̃A���t�@�l
/// </returns>
float HitEffect::GetAlpha() const {
	// �ŏ����玞�Ԃ�5����1�̎��Ԃŕs�����܂ŕω�������
	if (m_lifeTime / m_startLifeTime > 0.8f) {
		float t = (m_startLifeTime - m_lifeTime) / m_startLifeTime / (1 - 0.8f);
		t = t * (2 - t);
		return t;
	}
	// �I��肩�玞�Ԃ�2����1�̎��Ԃœ����܂ŕω�������
	else if (m_lifeTime / m_startLifeTime < 0.5f) {
		float t = m_lifeTime / m_startLifeTime / (1 - 0.5f);
		t = t * (2 - t);
		return t;
	}
	// ����ȊO�͕s����
	else {
		return 1.0f;
	}
}
