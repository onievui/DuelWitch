#pragma once
#ifndef FIRE_MAGIC_SHOOTER_DEFINED
#define FIRE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;


/// <summary>
/// 魔法発射クラス
/// </summary>
class FireMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	FireMagicShooter(MagicManager* magicManager);

public:
	// 魔法を更新する
	virtual void Create(MagicFactory* magicFactory, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);

private:
	// 魔法マネージャへのポインタ
	MagicManager* m_pMagicManager;

};


#endif // !FIRE_MAGIC_SHOOTER_DEFINED


