#pragma once
#ifndef THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED
#define THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;
enum class PlayerID;


/// <summary>
/// 落雷魔法発射クラス
/// </summary>
class ThunderStrikeMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	ThunderStrikeMagicShooter(MagicManager* magicManager);

public:
	// 落雷魔法を発射する
	void Create(int level, MagicFactory* magicFactory, PlayerID playerId,
		const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 魔法マネージャへのポインタ
	MagicManager* m_pMagicManager;

};


#endif // !THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED

