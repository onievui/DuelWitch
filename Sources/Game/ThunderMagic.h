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
	ThunderMagic(MagicManager* magicManager);
	~ThunderMagic();

public:
	// 雷魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 雷魔法を開放する
	void Lost() override;
	// 雷魔法を生成する
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) override;
	// 雷魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// プレイヤーとの衝突処理
	void HitPlayer(const SphereCollider& collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

private:
	// 魔法マネージャへのポインタ
	MagicManager*                                m_pMagicManager;
	// 落雷生成用タイマー作動フラグ
	bool                                         m_startTimer;
	// 落雷生成用タイマー
	float                                        m_time;
};


#endif // !THUNDER_MAGIC_DEFINED

