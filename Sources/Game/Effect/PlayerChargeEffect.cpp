#include "PlayerChargeEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\Color.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// エフェクトを初期化する
/// </summary>
/// <param name="lifeTime">ループするまでの時間</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="accel">加速度</param>
void PlayerChargeEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	Effect::Initialize(lifeTime, pos, vel, accel);
	m_delayTime = 0.0f;
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
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

	// 時間が無くなったらループする
	if (m_lifeTime < 0) {
		Restart();
	}
}

/// <summary>
/// ループする
/// </summary>
void PlayerChargeEffect::Restart() {
	m_lifeTime = m_startLifeTime;
}

/// <summary>
/// スケールを取得する
/// </summary>
/// <returns>
/// エフェクトのスケール
/// </returns>
float PlayerChargeEffect::GetScale() const {
	const EffectParameter::player_charge_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	return Math::Lerp(parameter.minScale, parameter.maxScale, m_lifeTime / m_startLifeTime);
}


