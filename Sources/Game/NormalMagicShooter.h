#pragma once
#ifndef NORAML_MAGIC_SHOOTER_DEFINED
#define NORAML_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// 通常魔法発射クラス
/// </summary>
class NormalMagicShooter : public IMagicShooter {
public:
	// コンストラクタ
	NormalMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// 通常魔法を発射する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// 生成した魔法の登録先へのポインタ
	std::vector<IMagic*>* m_pMagics;
	// 魔法ファクトリへのポインタ
	MagicFactory* m_pMagicFactory;

};


#endif // !NORMAL_MAGIC_SHOOTER_DEFINED


