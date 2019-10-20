#pragma once
#ifndef FREEZE_MAGIC_SHOOTER_DEFINED
#define FREEZE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// 氷魔法発射クラス
/// </summary>
class FreezeMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	FreezeMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// 氷魔法を発射する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 生成した魔法の登録先へのポインタ
	std::vector<IMagic*>* m_pMagics;
	// 魔法ファクトリへのポインタ
	MagicFactory* m_pMagicFactory;

};


#endif // !FREEZE_MAGIC_SHOOTER_DEFINED

