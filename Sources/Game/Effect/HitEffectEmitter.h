#pragma once
#ifndef HIT_EFFECT_EMITTER_DEFINED
#define HIT_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class HitEffect;


/// <summary>
/// ヒットエフェクトエミッタークラス
/// </summary>
class HitEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	HitEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:
	// エフェクト
	std::vector<std::unique_ptr<HitEffect>> m_effects;
	// 視線ベクトル
	DirectX::SimpleMath::Vector3 m_eyeVec;
};


#endif // !HIT_EFFECT_EMITTER_DEFINED
