#pragma once
#ifndef FIELD_SHIELD_EFFECT_DEFINED
#define FIELD_SHIELD_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// フィールド衝突エフェクト
/// </summary>
class FieldShieldEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;

public:
	// スケールを取得する
	float GetScale() const;
	// 色を取得する
	DirectX::SimpleMath::Color GetColor() const;
	// 使用しているかどうか取得する
	bool IsUsed() const { return m_isUsed; }

private:
	// 使用しているかどうかのフラグ
	bool m_isUsed;

};


#endif // !FIELD_SHIELD_EFFECT_DEFINED
