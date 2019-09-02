#include "MagicManager.h"
#include "MagicFactory.h"
#include "MagicID.h"
#include "ElementFactory.h"
#include "IMagic.h"
#include "IMagicShooter.h"
#include "NormalMagicShooter.h"
#include "FireMagicShooter.h"
#include "ThunderMagicShooter.h"
#include "ThunderStrikeMagicShooter.h"
#include "FreezeMagicShooter.h"


/// <summary>
/// コンストラクタ
/// </summary>
MagicManager::MagicManager() 
	: m_magics()
	, m_magicFactory()
	, m_magicShooters() {

}


/// <summary>
/// デストラクタ
/// </summary>
MagicManager::~MagicManager() {
}


/// <summary>
/// 魔法マネージャを初期化する
/// </summary>
void MagicManager::Initialize() {
	m_magicFactory = std::make_unique<MagicFactory>();
	m_magicFactory->Initialize(this);
	m_magics.clear();
	m_magics.resize(m_magicFactory->GetMagicMaxNum(), nullptr);
	m_magicShooters.resize(static_cast<int>(MagicID::Num));
	m_magicShooters[static_cast<int>(MagicID::Normal)]        = std::make_unique<NormalMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::Fire)]          = std::make_unique<FireMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::Thunder)]       = std::make_unique<ThunderMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::ThunderStrike)] = std::make_unique<ThunderStrikeMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::Freeze)]        = std::make_unique<FreezeMagicShooter>(this);
}


/// <summary>
/// 魔法マネージャを更新する
/// </summary>
/// <param name="timer">タイマー</param>
void MagicManager::Update(const DX::StepTimer& timer) {
	for (std::vector<IMagic*>::iterator itr = m_magics.begin(); itr != m_magics.end(); ++itr) {
		if (*itr) {
			(*itr)->Update(timer);
			if (!(*itr)->IsUsed()) {
				*itr = nullptr;
			}
		}
	}
}


/// <summary>
/// 魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void MagicManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (std::vector<IMagic*>::const_iterator itr = m_magics.cbegin(); itr != m_magics.cend(); ++itr) {
		if (*itr) {
			(*itr)->Render(view, proj);
		}
	}
}


/// <summary>
/// 魔法を生成する
/// </summary>
/// <param name="id">魔法ID</param>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="vec">向き</param>
void MagicManager::CreateMagic(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	m_magicShooters[static_cast<int>(id)]->Create(m_magicFactory.get(), playerId, pos, dir);
}

/// <summary>
/// 魔法を生成する
/// </summary>
/// <param name="id">エレメントID</param>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void MagicManager::CreateMagic(ElementID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	CreateMagic(ElementID2MagicID(id), playerId, pos, dir);
}


/// <summary>
/// 魔法を取得する
/// </summary>
/// <returns>
/// 魔法の配列
/// </returns>
std::vector<IMagic*>* MagicManager::GetMagics() {
	return &m_magics;
}

/// <summary>
/// エレメントIDから魔法IDに変換する
/// </summary>
/// <param name="elementId">エレメントID</param>
/// <returns>
/// 魔法ID
/// </returns>
MagicID MagicManager::ElementID2MagicID(ElementID elementId) {
	switch (elementId) {
	case ElementID::Fire:
		return MagicID::Fire;
	case ElementID::Thunder:
		return MagicID::Thunder;
	case ElementID::Freeze:
		return MagicID::Freeze;
	default:
		return MagicID::Normal;
	}
}
