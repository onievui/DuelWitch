#pragma once
#ifndef PLAYER_TRAIL_EFFECT_DEFINED
#define PLAYER_TRAIL_EFFECT_DEFINED


#include "Effect.h"


class Transform;


/// <summary>
/// プレイヤーの軌道エフェクト
/// </summary>
class PlayerTrailEffect : public Effect {
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
	// 親オブジェクトを設定する
	void SetParent(const Transform* pParent) { m_pParent = pParent; }
	// スケールを取得する
	float GetScale() const;
	// 色を取得する
	DirectX::SimpleMath::Color GetColor() const;
	// 開始遅延時間を設定する
	void SetDelayTime(float delayTime) { m_delayTime = delayTime; }
	// 開始待ちかどうか取得する
	bool IsWaiting() const { return m_delayTime > 0.0f; }

private:
	// 親オブジェクト
	const Transform*           m_pParent;
	// 色
	DirectX::SimpleMath::Color m_color;
	// 初期サイズ
	float                      m_defaultSize;
	// 開始遅延時間
	float                      m_delayTime;
};


#endif // !PLAYER_TRAIL_EFFECT_DEFINED
