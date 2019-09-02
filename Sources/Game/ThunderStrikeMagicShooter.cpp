#include "ThunderStrikeMagicShooter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
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
	DirectX::SimpleMath::Vector3 position = pos;
	position.y += ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam.appearPosY;
	for (auto& magic : *m_pMagicManager->GetMagics()) {
		if (!magic) {
			magic = magicFactory->Create(MagicID::ThunderStrike, playerId, position, dir);
			break;
		}
	}
}
