#include "MagicManager.h"
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Game\Element\ElementFactory.h>
#include "MagicFactory.h"
#include "MagicID.h"
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
	// 魔法ファクトリを初期化する
	m_magicFactory = std::make_unique<MagicFactory>();
	m_magicFactory->Initialize(this);

	// 生成済み魔法を初期化する
	m_magics.clear();
	m_magics.resize(m_magicFactory->GetMagicMaxNum(), nullptr);

	// 魔法発射クラスを初期化する
	m_magicShooters.resize(static_cast<int>(MagicID::Num));
	m_magicShooters[static_cast<int>(MagicID::Normal)]        = std::make_unique<NormalMagicShooter>(&m_magics, m_magicFactory.get());
	m_magicShooters[static_cast<int>(MagicID::Fire)]          = std::make_unique<FireMagicShooter>(&m_magics, m_magicFactory.get());
	m_magicShooters[static_cast<int>(MagicID::Thunder)]       = std::make_unique<ThunderMagicShooter>(&m_magics, m_magicFactory.get());
	m_magicShooters[static_cast<int>(MagicID::ThunderStrike)] = std::make_unique<ThunderStrikeMagicShooter>(&m_magics, m_magicFactory.get());
	m_magicShooters[static_cast<int>(MagicID::Freeze)]        = std::make_unique<FreezeMagicShooter>(&m_magics, m_magicFactory.get());
}


/// <summary>
/// 魔法マネージャを更新する
/// </summary>
/// <param name="timer">タイマー</param>
void MagicManager::Update(const DX::StepTimer& timer) {
	for (IfIterator<std::vector<IMagic*>> itr(m_magics, LamdaUtils::NotNull()); itr != m_magics.end(); ++itr) {
		(*itr)->Update(timer);
		// 使われなくなったら終了処理を行う
		if (!(*itr)->IsUsed()) {
			(*itr)->Finalize();
			*itr = nullptr;
		}
	}
}


/// <summary>
/// 魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void MagicManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (IfIterator<std::vector<IMagic*>> itr(m_magics, LamdaUtils::NotNull()); itr != m_magics.end(); ++itr) {
		(*itr)->Render(view, proj);
	}
}


/// <summary>
/// 魔法を生成する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="vec">向き</param>
void MagicManager::CreateMagic(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	m_magicShooters[static_cast<int>(magicInfo.id)]->Create(magicInfo, pos, dir);
}

/// <summary>
/// 魔法を生成する
/// </summary>
/// <param name="id">エレメントID</param>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">向き</param>
void MagicManager::CreateMagic(ElementID id, const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	MagicInfo info = magicInfo;
	info.id = ElementID2MagicID(id);
	CreateMagic(info, pos, dir);
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
