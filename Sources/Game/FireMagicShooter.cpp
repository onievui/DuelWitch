#include <Framework\DirectX11.h>
#include "FireMagicShooter.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager"></param>
FireMagicShooter::FireMagicShooter(MagicManager* magicManager)
	: m_pMagicManager(magicManager) {
}

/// <summary>
/// �����@�𔭎˂���
/// </summary>
/// <param name="magicFactory">���@�t�@�N�g���ւ̃|�C���^</param>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void FireMagicShooter::Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	constexpr float angle = Math::PI / 36;
	int count = 0;
	auto quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, -angle);
	auto direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
	quaternion.Inverse(quaternion);
	for (auto& magic : *m_pMagicManager->GetMagics()) {
		if (!magic) {
			magic = magicFactory->Create(MagicID::Fire, playerId, pos, direction);
			++count;
			if (count == 3) {
				break;
			}
			direction = DirectX::SimpleMath::Vector3::Transform(direction, quaternion);
		}
	}
}
