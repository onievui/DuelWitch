#include "FireMagicShooter.h"
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
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
/// <param name="magicManager">魔法マネージャへのポインタ</param>
FireMagicShooter::FireMagicShooter(MagicManager* pMagicManager)
	: m_pMagicManager(pMagicManager) {
}

/// <summary>
/// 炎魔法を発射する
/// </summary>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void FireMagicShooter::Create(MagicFactory* magicFactory, const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	const float angle = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam.wayAngle;
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::UnitY, -angle);
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
	quaternion.Inverse(quaternion);

	std::vector<IMagic*>* magics = m_pMagicManager->GetMagics();
	int count = 0;
	// 3方向に発射する
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*magics, LamdaUtils::IsNull());
		itr != magics->end() && count < 3;
		LamdaUtils::FindIfNext(itr, magics->end(), LamdaUtils::IsNull()), ++count) {
		(*itr) = magicFactory->Create(magicInfo, pos, direction);
		// 発射するたびに向きをずらす
		direction = DirectX::SimpleMath::Vector3::Transform(direction, quaternion);
	}
	
}
