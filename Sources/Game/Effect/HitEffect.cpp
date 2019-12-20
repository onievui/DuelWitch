#include "HitEffect.h"
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
void HitEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	Effect::Initialize(lifeTime, pos, vel, accel);
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer"></param>
void HitEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_lifeTime -= elapsed_time;

}

/// <summary>
/// スケールを取得する
/// </summary>
/// <returns>
/// パーティクルのスケール
/// </returns>
float HitEffect::GetScale() const {
	EffectParameter::hit_param parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->hitParam;

	// 最初から2分の1の時間で最大スケールまで変化させる
	if (m_lifeTime / m_startLifeTime > 0.5f) {
		float t = (m_startLifeTime - m_lifeTime) / m_startLifeTime / (1 - 0.5f);
		t = t * (2 - t);
		return parameter.scale * t;
	}

	return parameter.scale;
}

/// <summary>
/// アルファを取得する
/// </summary>
/// <returns>
/// パーティクルのアルファ値
/// </returns>
float HitEffect::GetAlpha() const {
	// 最初から時間の5分の1の時間で不透明まで変化させる
	if (m_lifeTime / m_startLifeTime > 0.8f) {
		float t = (m_startLifeTime - m_lifeTime) / m_startLifeTime / (1 - 0.8f);
		t = t * (2 - t);
		return t;
	}
	// 終わりから時間の2分の1の時間で透明まで変化させる
	else if (m_lifeTime / m_startLifeTime < 0.5f) {
		float t = m_lifeTime / m_startLifeTime / (1 - 0.5f);
		t = t * (2 - t);
		return t;
	}
	// それ以外は不透明
	else {
		return 1.0f;
	}
}
