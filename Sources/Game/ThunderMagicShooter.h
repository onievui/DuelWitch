#pragma once
#ifndef THUNDER_MAGIC_SHOOTER_DEFINED
#define THUNDER_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;
enum class PlayerID;


/// <summary>
/// 雷魔法発射クラス
/// </summary>
class ThunderMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	ThunderMagicShooter(MagicManager* magicManager);

public:
	// 雷魔法を発射する
	void Create(MagicFactory* magicFactory, const MagicInfo& magicInfo,
		const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 魔法マネージャへのポインタ
	MagicManager* m_pMagicManager;

};


#endif // !THUNDER_MAGIC_SHOOTER_DEFINED

