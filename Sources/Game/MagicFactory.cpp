#include "MagicFactory.h"
#include "IMagic.h"
#include "NormalMagic.h"


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
	m_magics.resize(MAGIC_MAX_NUM);
	for (auto& magic : m_magics) {
		magic = std::make_unique<NormalMagic>();
	}
}

/// <summary>
/// 魔法の生成処理
/// </summary>
/// <param name="id">魔法ID</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
/// <returns>
/// 魔法
/// </returns>
IMagic* MagicFactory::Create(MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	int index = 0;
	for (auto& magic : m_magics) {
		if (!magic->IsUsed()) {
			break;
		}
		++index;
	}

	if (index == MAGIC_MAX_NUM) {
		return nullptr;
	}

	switch (id) {
	case MagicID::Normal:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::White));
		break;
	case MagicID::Fire:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Red));
		break;
	case MagicID::Thunder:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Yellow));
		break;
	case MagicID::Freeze:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::SkyBlue));
		break;
	default:
		return nullptr;
	}

	m_magics[index]->IsUsed(true);

	return m_magics[index].get();
}

