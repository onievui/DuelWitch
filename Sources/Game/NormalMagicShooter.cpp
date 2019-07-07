#include "NormalMagicShooter.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager"></param>
NormalMagicShooter::NormalMagicShooter(MagicManager* magicManager)
	: m_pMagicManager(magicManager) {
}

/// <summary>
/// 魔法の生成
/// </summary>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void NormalMagicShooter::Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	for (auto& magic : *m_pMagicManager->GetMagics()) {
		if (!magic) {
			magic = magicFactory->Create(MagicFactory::MagicID::Normal, playerId, pos, dir);
			break;
		}
	}
}
