#include "ThunderMagicShooter.h"
#include <Utils\LamdaUtils.h>
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
	// 水平に発射する
	DirectX::SimpleMath::Vector3 direction = dir;
	direction.y = 0.0f;

	std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_pMagics, LamdaUtils::IsNull());
	if (itr != m_pMagics->end()) {
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, dir);
	}
	
}
