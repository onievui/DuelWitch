#include "ThunderStrikeMagicShooter.h"
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\AudioManager.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicFactory.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magics">生成した魔法の登録先へのポインタ</param>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
ThunderStrikeMagicShooter::ThunderStrikeMagicShooter(std::vector<IMagic*>* pMagics, MagicFactory* pMagicFactory)
	: m_pMagics(pMagics)
	, m_pMagicFactory(pMagicFactory) {
}

/// <summary>
/// 落雷魔法を発射する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void ThunderStrikeMagicShooter::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	DirectX::SimpleMath::Vector3 position = pos;
	position.y += ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam.appearPosY;
	
	std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_pMagics, LamdaUtils::IsNull());
	if (itr != m_pMagics->end()) {
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, dir);
	}

	// 効果音を鳴らす
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::ThunderStrikeMagic);
}
