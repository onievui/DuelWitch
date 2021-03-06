#pragma once
#ifndef EFFECT_MANAGER_DEFINED
#define EFFECT_MANAGER_DEFINED


#include <Framework\StepTimer.h>
#include "EffectID.h"


class IEffectEmitter;
class EffectFactory;
class Camera;


/// <summary>
/// エフェクトマネージャクラス
/// </summary>
class EffectManager {
public:
	// コンストラクタ
	EffectManager();
	// デストラクタ
	~EffectManager();

public:
	// エフェクトマネージャを初期化する
	void Initialize();
	// エフェクトマネージャを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera);
	// エフェクトを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// エフェクトを生成する
	IEffectEmitter* CreateEffect(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);

private:
	// 生成済みエフェクト
	std::vector<IEffectEmitter*>                                                  m_effects;
	// エフェクトファクトリ
	std::unique_ptr<EffectFactory>                                                m_effectFactory;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

};


#endif // !EFFECT_MANAGER_DEFINED
