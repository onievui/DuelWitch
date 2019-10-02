#include "FieldShieldEffect.h"
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"


/// <summary>
/// �G�t�F�N�g������������
/// </summary>
/// <param name="lifeTime">������܂ł̎���</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="accel">�����x</param>
void FieldShieldEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	vel, accel;

	m_startLifeTime = m_lifeTime = lifeTime;
	m_pos = pos;
	m_isUsed = true;
}

void FieldShieldEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_lifeTime -= elapsed_time;

	// ���Ԃ������Ȃ����疢�g�p�ɂ���
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
}

/// <summary>
/// �X�P�[�����擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̃X�P�[��
/// </returns>
float FieldShieldEffect::GetScale() const {
	const EffectParameter::field_shield_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam;
	return parameter.scale + (m_startLifeTime - m_lifeTime)*parameter.scaleSpeed;
}

/// <summary>
/// �F���擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̐F
/// </returns>
DirectX::SimpleMath::Color FieldShieldEffect::GetColor() const {
	DirectX::SimpleMath::Color color = DirectX::Colors::Aqua;
	float t = m_lifeTime / m_startLifeTime;
	float t2 = (t < 0.5f ? t2 = (0.5f - t)*2.0f : (t - 0.5f)*2.0f);

	color.w = 1.0f - t2*t2*t2;

	return color;
}
