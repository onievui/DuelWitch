#include "MagicFactory.h"
#include "IMagic.h"
#include "NormalMagic.h"
#include "FireMagic.h"
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
void MagicFactory::Initialize() {
	m_magics.clear();
	m_magics.resize(GetAllMagicMaxNum());
	for (auto itr = m_magics.begin() + MagicBeginIndex[(int)MagicID::Normal], end = itr + MagicMaxNum[(int)MagicID::Normal];
		itr != end; ++itr) {
		*itr = std::make_unique<NormalMagic>();
	}
	for (auto itr = m_magics.begin() + MagicBeginIndex[(int)MagicID::Fire], end = itr + MagicMaxNum[(int)MagicID::Fire];
		itr != end; ++itr) {
		*itr = std::make_unique<FireMagic>();
	}
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
	auto begin = m_magics.begin() + MagicBeginIndex[(int)id];
	auto end = begin + MagicMaxNum[(int)id];
	auto itr = std::find_if(begin, end, [](auto& element) {return !element->IsUsed(); });

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
	case MagicID::Freeze:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::SkyBlue));
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
int MagicFactory::GetAllMagicMaxNum() {
	int total = 0;
	for (auto& num : MagicMaxNum) {
		total += num;
	}
	return total;
}

