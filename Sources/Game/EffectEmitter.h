#pragma once
#ifndef EFFECT_EMITTER_DEFINED
#define EFFECT_EMITTER_DEFINED


#include "IEffectEmitter.h"
#include "Transform.h"


/// <summary>
/// エフェクトエミッタークラス
/// </summary>
class EffectEmitter : public IEffectEmitter {
public:
	// コンストラクタ
	EffectEmitter() 
		: m_cBuffer()
		, m_transform(nullptr)
		, m_pParent()
		, m_isUsed(false)
		, m_lifeTime(0) {
	}

public:
	// エフェクトエミッターを生成する
	virtual void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override { pos, dir; }
	// エフェクトエミッターを更新する
	virtual void Update(const DX::StepTimer& timer, const Camera* camera) override { timer, camera; }
	// エフェクトエミッターを描画する
	virtual void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override
	{ batch, view, proj; }

public:
	// 親を設定する
	void SetParent(const Transform* parent) override { m_pParent = parent; }
	// エフェクトを使用しているかどうか取得する
	bool IsUsed() const override { return m_isUsed; }
	// エフェクトを使用するかどうか設定する
	void SetUsed(bool isUsed) override { m_isUsed = isUsed; }

protected:
	//  定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>      m_cBuffer;
	// 姿勢
	ChildTransform                            m_transform;
	// 親の姿勢
	const Transform*                          m_pParent;
	// 使用しているかどうか
	bool                                      m_isUsed;
	// 生存時間
	float                                     m_lifeTime;
};


#endif // !EFFECT_EMITTER_DEFINED
