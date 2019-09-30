#include "Effect.h"


/// <summary>
/// �G�t�F�N�g������������
/// </summary>
/// <param name="lifeTime">���[�v����܂ł̎���</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="accel">�����x</param>
void Effect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	m_startPos = m_pos = pos;
	m_startVel = m_vel = vel;
	m_startAccel = m_accel = accel;
	m_startLifeTime = m_lifeTime = lifeTime;
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Effect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

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
void Effect::Restart() {
	m_pos = m_startPos;
	m_vel = m_startVel;
	m_accel = m_startAccel;
	m_lifeTime = m_startLifeTime;
}
