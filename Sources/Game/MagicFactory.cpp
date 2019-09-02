#include "MagicFactory.h"
#include <Utils\ServiceLocater.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "IMagic.h"
#include "NormalMagic.h"
#include "FireMagic.h"
#include "ThunderMagic.h"
#include "ThunderStrikeMagic.h"
#include "FreezeMagic.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
MagicFactory::MagicFactory() {
}

/// <summary>
/// デストラクタ
/// </summary>
MagicFactory::~MagicFactory() {
}


/// <summary>
/// 魔法ファクトリを初期化する
/// </summary>
/// <param name="magicManager">魔法マネージャ</param>
void MagicFactory::Initialize(MagicManager* magicManager) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	if (static_cast<int>(MagicID::Num) != parameter->maxNum.size()) {
		ErrorMessage(L"ロードした魔法の種類数が不正な値です");
	}
	m_maxNum = parameter->maxNum;
	m_beginIndex.resize(m_maxNum.size());
	m_beginIndex[0] = 0;
	for (unsigned int i = 1; i < m_beginIndex.size(); ++i) {
		m_beginIndex[i] = m_beginIndex[i - 1] + m_maxNum[i - 1];
	}

	m_magics.clear();
	m_magics.resize(GetMagicMaxNum());

	InitializeMagic<NormalMagic>(MagicID::Normal);
	InitializeMagic<FireMagic>(MagicID::Fire);
	InitializeMagic<ThunderMagic>(MagicID::Thunder, magicManager);
	InitializeMagic<ThunderStrikeMagic>(MagicID::ThunderStrike);
	InitializeMagic<FreezeMagic>(MagicID::Freeze);
}

/// <summary>
/// 魔法の生成処理
/// </summary>
/// <param name="id">魔法ID</param>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
/// <returns>
/// 魔法
/// </returns>
IMagic* MagicFactory::Create(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// 使用していないオブジェクトを探す
	std::vector<std::unique_ptr<IMagic>>::iterator begin = m_magics.begin() + m_beginIndex[static_cast<int>(id)];
	std::vector<std::unique_ptr<IMagic>>::iterator end = begin + m_maxNum[static_cast<int>(id)];
	std::vector<std::unique_ptr<IMagic>>::iterator itr = std::find_if(begin, end,
		[](std::unique_ptr<IMagic>& element) {return !element->IsUsed(); });

	// これ以上生成できないならnullptrを返す
	if (itr == end) {
		return nullptr;
	}

	switch (id) {
	case MagicID::Normal:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::White));
		break;
	case MagicID::Fire:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Red));
		break;
	case MagicID::Thunder:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Yellow));
		break;
	case MagicID::ThunderStrike:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Yellow));
		break;
	case MagicID::Freeze:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::SkyBlue) +
			DirectX::SimpleMath::Vector4(0,0,0,-0.8f));
		break;
	default:
		return nullptr;
	}

	(*itr)->IsUsed(true);

	return itr->get();
}

/// <summary>
/// 全魔法の最大出現数を取得する
/// </summary>
/// <returns>
/// 全魔法の最大出現数
/// </returns>
int MagicFactory::GetMagicMaxNum() {
	int total = 0;
	for (int i = 0; i < static_cast<int>(MagicID::Num); ++i) {
		total += m_maxNum[i];
	}	
	return total;
}

