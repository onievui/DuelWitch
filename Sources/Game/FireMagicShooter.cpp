#include <Framework\DirectX11.h>
#include "FireMagicShooter.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager"></param>
FireMagicShooter::FireMagicShooter(MagicManager* magicManager)
	: m_pMagicManager(magicManager) {
}

/// <summary>
/// 魔法の生成
/// </summary>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void FireMagicShooter::Create(MagicFactory* magicFactory, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	constexpr float angle = Math::PI / 36;
	int count = 0;
	auto quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, -angle);
	auto direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
	quaternion.Inverse(quaternion);
	for (auto& magic : *m_pMagicManager->GetMagics()) {
		if (!magic) {
			magic = magicFactory->Create(MagicFactory::MagicID::Fire, pos, direction);
			++count;
			if (count == 3) {
				break;
			}
			direction = DirectX::SimpleMath::Vector3::Transform(direction, quaternion);
		}
	}
}
