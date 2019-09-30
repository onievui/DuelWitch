#include "FireMagicEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"


/// <summary>
/// エフェクトを初期化する
/// </summary>
/// <param name="lifeTime">ループするまでの時間</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="accel">加速度</param>
void FireMagicEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	m_startPos = m_pos = pos;
	m_startVel = m_vel = vel;
	m_startAccel = m_accel = accel;
	m_startLifeTime = m_lifeTime = lifeTime;
	
	// 角度をランダムに決める
	m_angle = RandMt::GetRand(Math::PI2);
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void FireMagicEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	
	m_vel += m_accel * elapsed_time;
	m_pos += m_vel * elapsed_time;

	m_lifeTime -= elapsed_time;

	// 時間が無くなるか、一定以上動いたらループする
	if (m_lifeTime < 0 || (m_pos - m_startPos).Length() > 5.0f) {
		Restart();
	}
}

/// <summary>
/// ループする
/// </summary>
void FireMagicEffect::Restart() {
	m_pos = m_startPos;
	m_vel = m_startVel;
	m_accel = m_startAccel;
	m_lifeTime = m_startLifeTime;
	m_angle = RandMt::GetRand(Math::PI2);
}

/// <summary>
/// スケールを取得する
/// </summary>
/// <returns>
/// エフェクトのスケール
/// </returns>
float FireMagicEffect::GetScale() const {
	return m_lifeTime / m_startLifeTime + 0.1f;
}

/// <summary>
/// 色を取得する
/// </summary>
/// <returns>
/// エフェクトの色
/// </returns>
DirectX::SimpleMath::Color FireMagicEffect::GetColor() const {
	//return DirectX::SimpleMath::Color(1, static_cast<float>(0x58) / 0xff, static_cast<float>(0x1a) / 0xff, 1);
	DirectX::SimpleMath::Color color = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fireMagicParam.color;
	return color / 255.0f;
}
