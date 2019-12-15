#pragma once
#ifndef PLAYER_CHARGE_EFFECT_DEFINED
#define PLAYER_CHARGE_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// プレイヤーのチャージエフェクト
/// </summary>
class PlayerChargeEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;
	// ループする
	void Restart() override;

public:
	// スケールを取得する
	float GetScale() const;
	// 開始遅延時間を設定する
	void SetDelayTime(float delayTime) { m_delayTime = delayTime; }
	// 開始待ちかどうか取得する
	bool IsWaiting() const { return m_delayTime > 0.0f; }

private:
	// 開始遅延時間
	float                      m_delayTime;

};


#endif // !PLAYER_CHARGE_EFFECT_DEFINED
