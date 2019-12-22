#include "ThunderMagicShooter.h"
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Utils\MathUtils.h>
#include "MagicFactory.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magics">生成した魔法の登録先へのポインタ</param>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
ThunderMagicShooter::ThunderMagicShooter(std::vector<IMagic*>* pMagics, MagicFactory* pMagicFactory)
	: m_pMagics(pMagics)
	, m_pMagicFactory(pMagicFactory) {
}

/// <summary>
/// 雷魔法を発射する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void ThunderMagicShooter::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	if (magicInfo.level == 0) {
		std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_pMagics, LamdaUtils::IsNull());
		if (itr != m_pMagics->end()) {
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, dir);
		}
	}
	else if (magicInfo.level >= 1) {
		// TODO: 雷2way角度定数にする
		const float angle = Math::PI / 20;
		int count = 0;
		// 2方向に分岐させるための回転を作成する
		DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
		DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
			Math::CreateQuaternionFromVector3(xz_dir, dir));

		for (IfIterator<std::vector<IMagic*>> itr(*m_pMagics, LamdaUtils::IsNull()); itr != m_pMagics->end() && count < 2; ++itr, ++count) {
			DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, (count == 0 ? -angle : angle));
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
		}
	}
	
}
