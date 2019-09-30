#include "FireMagicEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"


/// <summary>
/// �G�t�F�N�g������������
/// </summary>
/// <param name="lifeTime">���[�v����܂ł̎���</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="accel">�����x</param>
void FireMagicEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	m_startPos = m_pos = pos;
	m_startVel = m_vel = vel;
	m_startAccel = m_accel = accel;
	m_startLifeTime = m_lifeTime = lifeTime;
	
	// �p�x�������_���Ɍ��߂�
	m_angle = RandMt::GetRand(Math::PI2);
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FireMagicEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	
	m_vel += m_accel * elapsed_time;
	m_pos += m_vel * elapsed_time;

	m_lifeTime -= elapsed_time;

	// ���Ԃ������Ȃ邩�A���ȏ㓮�����烋�[�v����
	if (m_lifeTime < 0 || (m_pos - m_startPos).Length() > 5.0f) {
		Restart();
	}
}

/// <summary>
/// ���[�v����
/// </summary>
void FireMagicEffect::Restart() {
	m_pos = m_startPos;
	m_vel = m_startVel;
	m_accel = m_startAccel;
	m_lifeTime = m_startLifeTime;
	m_angle = RandMt::GetRand(Math::PI2);
}

/// <summary>
/// �X�P�[�����擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̃X�P�[��
/// </returns>
float FireMagicEffect::GetScale() const {
	return m_lifeTime / m_startLifeTime + 0.1f;
}

/// <summary>
/// �F���擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̐F
/// </returns>
DirectX::SimpleMath::Color FireMagicEffect::GetColor() const {
	//return DirectX::SimpleMath::Color(1, static_cast<float>(0x58) / 0xff, static_cast<float>(0x1a) / 0xff, 1);
	DirectX::SimpleMath::Color color = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fireMagicParam.color;
	return color / 255.0f;
}
