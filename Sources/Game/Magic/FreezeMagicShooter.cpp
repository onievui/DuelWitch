#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Utils\MathUtils.h>
#include "FreezeMagicShooter.h"
#include "MagicFactory.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magics">生成した魔法の登録先へのポインタ</param>
/// <param name="magicFactory">魔法ファクトリへのポインタ</param>
FreezeMagicShooter::FreezeMagicShooter(std::vector<IMagic*>* pMagics, MagicFactory* pMagicFactory)
	: m_pMagics(pMagics)
	, m_pMagicFactory(pMagicFactory) {
}

/// <summary>
/// 氷魔法を発射する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void FreezeMagicShooter::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	IfIterator<std::vector<IMagic*>> itr(*m_pMagics, LamdaUtils::IsNull());
	if (itr != m_pMagics->end()) {
		// XZ方向に変換する
		DirectX::SimpleMath::Vector3 direction(dir.x, 0, dir.z);
		direction.Normalize();
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
	}
	else {
		return;
	}
	
	// チャージレベル1以上なら2つ出す（ハイスピード）
	if (magicInfo.level >= 1) {
		++itr;
		if (itr != m_pMagics->end()) {
			// XZ方向に変換する
			DirectX::SimpleMath::Vector3 direction(dir.x, 0, dir.z);
			direction.Normalize();
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, -direction);
		}
		else {
			return;
		}
	}

	// チャージレベル2ならさらに2つ出す（ハイスピード）
	if (magicInfo.level >= 2) {

		DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
			Math::CreateQuaternionFromVector3(xz_dir, dir));

		direction = DirectX::SimpleMath::Vector3::UnitY;

		for (int i = 0; i < 2; ++i) {
			++itr;
			if (itr != m_pMagics->end()) {
				(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
			}
			else {
				return;
			}
			direction *= -1;
		}
	}
}
