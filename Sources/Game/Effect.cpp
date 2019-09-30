#include "Effect.h"


/// <summary>
/// エフェクトを初期化する
/// </summary>
/// <param name="lifeTime">ループするまでの時間</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="accel">加速度</param>
void Effect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	m_startPos = m_pos = pos;
	m_startVel = m_vel = vel;
	m_startAccel = m_accel = accel;
	m_startLifeTime = m_lifeTime = lifeTime;
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Effect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_vel += m_accel * elapsed_time;
	m_pos += m_vel * elapsed_time;

	m_lifeTime -= elapsed_time;

	// 時間が無くなったらループする
	if (m_lifeTime < 0) {
		Restart();
	}
}

/// <summary>
/// ループする
/// </summary>
void Effect::Restart() {
	m_pos = m_startPos;
	m_vel = m_startVel;
	m_accel = m_startAccel;
	m_lifeTime = m_startLifeTime;
}
