#include "PlayParameterLoader.h"
#include <Utils\LoadDataManager.h>


/// <summary>
/// コンストラクタ
/// </summary>
PlayParameterLoader::PlayParameterLoader() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Register(&m_commandParameter);
	manager->Register(&m_aiCommandParameter);
	manager->Register(&m_elementParameter);
	manager->Register(&m_magicParameter);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayParameterLoader::~PlayParameterLoader() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Unregister(&m_commandParameter);
	manager->Unregister(&m_aiCommandParameter);
	manager->Unregister(&m_elementParameter);
	manager->Unregister(&m_magicParameter);
}

/// <summary>
/// パラメータを読み込む
/// </summary>
void PlayParameterLoader::Load() {
	LoadDataManager::GetIns()->Load(LoadDataID::PlayScene);
}

/// <summary>
/// パラメータを再読み込みする
/// </summary>
void PlayParameterLoader::Reload() {
}

/// <summary>
/// パラメータを開放する
/// </summary>
void PlayParameterLoader::Dispose() {
	LoadDataManager::GetIns()->Dispose(LoadDataID::PlayScene);
}

