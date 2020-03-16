#pragma once
#ifndef THUNDER_MAGIC_DEFINED
#define THUNDER_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class MagicManager;


/// <summary>
/// 雷魔法クラス
/// </summary>
class ThunderMagic : public Magic {
public:
	// コンストラクタ
	ThunderMagic(MagicManager* pMagicManager);
	// デストラクタ
	~ThunderMagic();

public:
	// 雷魔法を生成する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// 雷魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 雷魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	// 雷魔法の終了処理を行う
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
	MagicManager*                                m_pMagicManager;
	// ロックオン用タイマー
	float                                        m_lockOnTimer;
	// 落雷生成用タイマー作動フラグ
	bool                                         m_startTimer;
	// 落雷生成用タイマー
	float                                        m_time;

};


#endif // !THUNDER_MAGIC_DEFINED

