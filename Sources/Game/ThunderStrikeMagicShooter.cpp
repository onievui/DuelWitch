#include <Framework\DirectX11.h>
#include "ThunderStrikeMagicShooter.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager"></param>
ThunderStrikeMagicShooter::ThunderStrikeMagicShooter(MagicManager* magicManager)
	: m_pMagicManager(magicManager) {
}

/// <summary>
/// 落雷魔法を発射する
/// </summary>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void ThunderStrikeMagicShooter::Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	auto position = pos;
	position.y = 55.0f;
	for (auto& magic : *m_pMagicManager->GetMagics()) {
		if (!magic) {
			magic = magicFactory->Create(MagicID::ThunderStrike, playerId, position, dir*10.0f);
			break;
		}
	}
}
