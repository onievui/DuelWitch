#pragma once
#ifndef NORAML_MAGIC_SHOOTER_DEFINED
#define NORAML_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;


/// <summary>
/// 魔法発射クラス
/// </summary>
class NormalMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	NormalMagicShooter(MagicManager* magicManager);

public:
	// 魔法を更新する
	void Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 魔法マネージャへのポインタ
	MagicManager* m_pMagicManager;

};


#endif // !NORMAL_MAGIC_SHOOTER_DEFINED


