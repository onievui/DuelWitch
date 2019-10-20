#pragma once
#ifndef THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED
#define THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// 落雷魔法発射クラス
/// </summary>
class ThunderStrikeMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	ThunderStrikeMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// 落雷魔法を発射する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 生成した魔法の登録先へのポインタ
	std::vector<IMagic*>* m_pMagics;
	// 魔法ファクトリへのポインタ
	MagicFactory* m_pMagicFactory;

};


#endif // !THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED

