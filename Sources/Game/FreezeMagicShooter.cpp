#include <Utils\LamdaUtils.h>
#include "FreezeMagicShooter.h"
#include "MagicID.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager"></param>
FreezeMagicShooter::FreezeMagicShooter(MagicManager* magicManager)
	: m_pMagicManager(magicManager) {
}

/// <summary>
/// 氷魔法を発射する
/// </summary>
/// <param name="level">レベル</param>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void FreezeMagicShooter::Create(int level, MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	level;

	std::vector<IMagic*>* magics = m_pMagicManager->GetMagics();
	std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*magics, LamdaUtils::IsNull());
	if (itr != magics->end()) {
		(*itr) = magicFactory->Create(MagicID::Freeze, playerId, pos, dir);
	}
	
}
