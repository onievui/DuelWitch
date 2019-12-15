#pragma once
#ifndef PLAYER_CHARGE_EFFECT_EMITTER_DEFINED
#define PLAYER_CHARGE_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class PlayerChargeEffect;


/// <summary>
/// プレイヤーのチャージエフェクトエミッタークラス
/// </summary>
class PlayerChargeEffectEmitter : public EffectEmitter {
public:
	// チャージ状態
	enum class State {
		Charge0,	// 0段階
		Charge1,	// 1段階
		Charge2,	// 2段階

		None,		// チャージしていない
	};

public:
	// コンストラクタ
	PlayerChargeEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// 親オブジェクトを設定する
	void SetParent(const Transform* pParent) override;
	// チャージ状態を設定する
	void SetChargeState(State state) { m_chargeState = state; }

private:
	// 色を取得する
	DirectX::SimpleMath::Color GetColor() const;

private:
	// エフェクト
	std::vector<std::unique_ptr<PlayerChargeEffect>> m_effects;
	// 視線ベクトル
	DirectX::SimpleMath::Vector3                     m_eyeVec;
	// チャージ状態
	State                                            m_chargeState;

};


#endif // !PLAYER_CHARGE_EFFECT_EMITTER_DEFINED
