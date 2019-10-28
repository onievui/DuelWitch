#pragma once
#ifndef FIRE_MAGIC_SHOOTER_DEFINED
#define FIRE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// 炎魔法発射クラス
/// </summary>
class FireMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	FireMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// 炎魔法を発射する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 生成した魔法の登録先へのポインタ
	std::vector<IMagic*>* m_pMagics;
	// 魔法ファクトリへのポインタ
	MagicFactory* m_pMagicFactory;

};


#endif // !FIRE_MAGIC_SHOOTER_DEFINED


