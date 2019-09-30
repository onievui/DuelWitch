#pragma once
#ifndef PLAYER_TRAIL_EFFECT_EMITTER_DEFINED
#define PLAYER_TRAIL_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class PlayerTrailEffect;


/// <summary>
/// プレイヤーの軌道エフェクトエミッタークラス
/// </summary>
class PlayerTrailEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	PlayerTrailEffectEmitter();

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

private:
	// エフェクト
	std::vector<std::unique_ptr<PlayerTrailEffect>> m_effects;
	// 視線ベクトル
	DirectX::SimpleMath::Vector3                    m_eyeVec;
	// 初期の発射方向
	DirectX::SimpleMath::Vector3                    m_createDir;
};


#endif // !PLAYER_TRAIL_EFFECT_EMITTER_DEFINED
