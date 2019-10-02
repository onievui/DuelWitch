#pragma once
#ifndef FIELD_SHIELD_EFFECT_EMITTER_DEFINED
#define FIELD_SHIELD_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class FieldShieldEffect;


/// <summary>
/// フィールド衝突エフェクトエミッタークラス
/// </summary>
class FieldShieldEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	FieldShieldEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// エフェクトを追加する
	void AddEffect(const DirectX::SimpleMath::Vector3& pos);

private:
	// エフェクト
	std::vector<std::unique_ptr<FieldShieldEffect>> m_effects;

};


#endif // !FIELD_SHIELD_EFFECT_EMITTER_DEFINED
