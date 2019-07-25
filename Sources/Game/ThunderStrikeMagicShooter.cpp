#include <Framework\DirectX11.h>
#include "ThunderStrikeMagicShooter.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager"></param>
ThunderStrikeMagicShooter::ThunderStrikeMagicShooter(MagicManager* magicManager)
	: m_pMagicManager(magicManager) {
}

/// <summary>
/// �������@�𔭎˂���
/// </summary>
/// <param name="magicFactory">���@�t�@�N�g���ւ̃|�C���^</param>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
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
