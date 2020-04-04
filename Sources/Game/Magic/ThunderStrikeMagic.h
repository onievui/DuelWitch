#pragma once
#ifndef THUNDER_STRIKE_MAGIC_DEFINED
#define THUNDER_STRIKE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class MagicManager;
class IEffectEmitter;


/// <summary>
/// 落雷魔法クラス
/// </summary>
class ThunderStrikeMagic : public Magic {
public:
	// 氷魔法に引き寄せられる領域の半径
	static constexpr float ATTRACTED_RADIUS = 5.0f;
	// 氷魔法に引き寄せられる速度
	static constexpr float ATTRACTED_SPEED = 0.5f;

public:
	// コンストラクタ
	ThunderStrikeMagic(MagicManager* pMagicManager);
	// デストラクタ
	~ThunderStrikeMagic();

public:
	// 落雷魔法を生成する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// 落雷魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 落雷魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	// 落雷魔法の終了処理を行う
	void Finalize() override;

public:
	// ダメージを取得する
	float GetPower() const override;
	// プレイヤーとの衝突処理
	void HitPlayer(const Collider* collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

private:
	// 魔法マネージャへのポインタ
	MagicManager*   m_pMagicManager;
	// 落雷魔法エフェクト
	IEffectEmitter* m_pEffect;
	// 跳ね返った回数
	int             m_refrectCount;

};


#endif // !THUNDER_STRIKE_STRIKE_MAGIC_DEFINED
