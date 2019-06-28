#include "MagicManager.h"
#include "IMagic.h"


/// <summary>
/// コンストラクタ
/// </summary>
MagicManager::MagicManager() 
	: m_magics()
	, m_magicFactory() {

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
/// <param name="pos">座標</param>
/// <param name="vec">向き</param>
void MagicManager::CreateMagic(MagicFactory::MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vec) {
	IMagic* created_magic = m_magicFactory->Create(id, pos, vec);
	for (auto& magic : m_magics) {
		if (!magic) {
			magic = created_magic;
			break;
		}
	}
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
