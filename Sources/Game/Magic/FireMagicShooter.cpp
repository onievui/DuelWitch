#include "FireMagicShooter.h"
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicFactory.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magics">生成した魔法の登録先へのポインタ</param>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
FireMagicShooter::FireMagicShooter(std::vector<IMagic*>* pMagics, MagicFactory* pMagicFactory)
	: m_pMagics(pMagics)
	, m_pMagicFactory(pMagicFactory) {
}

/// <summary>
/// 炎魔法を発射する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void FireMagicShooter::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const float angle = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam.wayAngle;
	// 3方向に分岐させるための回転を作成する
	DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
	DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
		Math::CreateQuaternionFromVector3(xz_dir, dir));

	//DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::UnitY;
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, -angle);
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
	quaternion.Inverse(quaternion);

	int count = 0;
	// 3方向に発射する
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_pMagics, LamdaUtils::IsNull());
		itr != m_pMagics->end() && count < 3;
		LamdaUtils::FindIfNext(itr, m_pMagics->end(), LamdaUtils::IsNull()), ++count) {
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
		// 発射するたびに向きをずらす
		direction = DirectX::SimpleMath::Vector3::Transform(direction, quaternion);
	}
	
}
