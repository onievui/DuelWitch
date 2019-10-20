#pragma once
#ifndef THUNDER_MAGIC_SHOOTER_DEFINED
#define THUNDER_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// 雷魔法発射クラス
/// </summary>
class ThunderMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	ThunderMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// 雷魔法を発射する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 生成した魔法の登録先へのポインタ
	std::vector<IMagic*>* m_pMagics;
	// 魔法ファクトリへのポインタ
	MagicFactory* m_pMagicFactory;

};


#endif // !THUNDER_MAGIC_SHOOTER_DEFINED

