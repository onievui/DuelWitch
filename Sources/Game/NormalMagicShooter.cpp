#include "NormalMagicShooter.h"
#include <Utils\LamdaUtils.h>
#include "MagicID.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager">魔法マネージャへのポインタ</param>
NormalMagicShooter::NormalMagicShooter(MagicManager* pMagicManager)
	: m_pMagicManager(pMagicManager) {
}

/// <summary>
/// 通常魔法を発射する
/// </summary>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void NormalMagicShooter::Create(MagicFactory* magicFactory, const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	std::vector<IMagic*>* magics = m_pMagicManager->GetMagics();
	std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*magics, LamdaUtils::IsNull());
	if (itr != magics->end()) {
		(*itr) = magicFactory->Create(magicInfo, pos, dir);
	}
	
}
