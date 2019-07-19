#pragma once
#ifndef FREEZE_MAGIC_SHOOTER_DEFINED
#define FREEZE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;
enum class PlayerID;


/// <summary>
/// 氷魔法発射クラス
/// </summary>
class FreezeMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	FreezeMagicShooter(MagicManager* magicManager);

public:
	// 氷魔法を発射する
	void Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 魔法マネージャへのポインタ
	MagicManager* m_pMagicManager;

};


#endif // !FREEZE_MAGIC_SHOOTER_DEFINED

