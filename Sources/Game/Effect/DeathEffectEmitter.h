#pragma once
#ifndef DEATH_EFFECT_EMITTER_DEFINED
#define DEATH_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class DeathEffect;


/// <summary>
/// 撃破エフェクトエミッタークラス
/// </summary>
class DeathEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	DeathEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// エフェクトの色番号を設定する
	void SetColorID(int id);

private:
	// エフェクト
	std::vector<std::unique_ptr<DeathEffect>> m_effects;
	// 視線ベクトル
	DirectX::SimpleMath::Vector3              m_eyeVec;
	// 色
	DirectX::SimpleMath::Color                m_color;
};


#endif // !DEATH_EFFECT_EMITTER_DEFINED
