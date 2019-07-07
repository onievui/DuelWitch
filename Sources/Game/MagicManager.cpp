#include "MagicManager.h"
#include "IMagic.h"
#include "IMagicShooter.h"
#include "NormalMagicShooter.h"
#include "FireMagicShooter.h"


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
	m_magics.clear();
	m_magics.resize(MagicFactory::MAGIC_MAX_NUM, nullptr);
	m_magicFactory = std::make_unique<MagicFactory>();
	m_magicFactory->Initialize();
	m_magicShooters.resize(2);
	m_magicShooters[(int)MagicFactory::MagicID::Normal] = std::make_unique<NormalMagicShooter>(this);
	m_magicShooters[(int)MagicFactory::MagicID::Fire]   = std::make_unique<FireMagicShooter>(this);
}


/// <summary>
/// 魔法マネージャを更新する
/// </summary>
/// <param name="timer">タイマー</param>
void MagicManager::Update(const DX::StepTimer& timer) {
	for (auto& magic : m_magics) {
		if (magic) {
			magic->Update(timer);
			if (!magic->IsUsed()) {
				magic = nullptr;
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
	for (const auto& magic : m_magics) {
		if (magic) {
			magic->Render(view, proj);
		}
	}
}


/// <summary>
/// 魔法を生成する
/// </summary>
/// <param name="id">魔法のID</param>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="vec">向き</param>
void MagicManager::CreateMagic(MagicFactory::MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	m_magicShooters[(int)id]->Create(m_magicFactory.get(), playerId, pos, dir);
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
