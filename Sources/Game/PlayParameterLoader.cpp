#include "PlayParameterLoader.h"
#include <Utils\LoadDataManager.h>
#include <Utils\LoadDataHolder.h>

#include <Parameters\CommandParameter.h>
#include <Parameters\AICommandParameter.h>
#include <Parameters\ElementParameter.h>
#include <Parameters\MagicParameter.h>


/// <summary>
/// ヘッダファイルでのインクルードを減らすためのクラス
/// </summary>
class PlayParameterLoader::Impl {
public:
	// コンストラクタ
	Impl();
	// デストラクタ
	~Impl();

public:
	// 読み込みデータ
	LoadDataHolder<CommandParameter, LoadDataID::PlayScene>   commandParameter;
	LoadDataHolder<AICommandParameter, LoadDataID::PlayScene> aiCommandParameter;
	LoadDataHolder<ElementParameter, LoadDataID::PlayScene>   elementParameter;
	LoadDataHolder<MagicParameter, LoadDataID::PlayScene>     magicParameter;
};

/// <summary>
/// コンストラクタ
/// </summary>
PlayParameterLoader::Impl::Impl() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Register(&commandParameter);
	manager->Register(&aiCommandParameter);
	manager->Register(&elementParameter);
	manager->Register(&magicParameter);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayParameterLoader::Impl::~Impl(){
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Unregister(&commandParameter);
	manager->Unregister(&aiCommandParameter);
	manager->Unregister(&elementParameter);
	manager->Unregister(&magicParameter);
}



/// <summary>
/// コンストラクタ
/// </summary>
PlayParameterLoader::PlayParameterLoader() {
	m_impl = std::make_unique<PlayParameterLoader::Impl>();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayParameterLoader::~PlayParameterLoader() {
	m_impl.reset();
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
	LoadDataManager::GetIns()->Reload(LoadDataID::PlayScene);
}

/// <summary>
/// パラメータを開放する
/// </summary>
void PlayParameterLoader::Dispose() {
	LoadDataManager::GetIns()->Dispose(LoadDataID::PlayScene);
}

/// <summary>
/// パラメータを取得する
/// </summary>
/// <returns>
/// パラメータへのポインタ
/// </returns>
const CommandParameter* PlayParameterLoader::GetCommandParameter() {
	return m_impl->commandParameter.Get();
}

/// <summary>
/// パラメータを取得する
/// </summary>
/// <returns>
/// パラメータへのポインタ
/// </returns>
const AICommandParameter* PlayParameterLoader::GetAICommandParameter() {
	return m_impl->aiCommandParameter.Get();
}

/// <summary>
/// パラメータを取得する
/// </summary>
/// <returns>
/// パラメータへのポインタ
/// </returns>
const ElementParameter* PlayParameterLoader::GetElementParameter() {
	return m_impl->elementParameter.Get();
}

/// <summary>
/// パラメータを取得する
/// </summary>
/// <returns>
/// パラメータへのポインタ
/// </returns>
const MagicParameter* PlayParameterLoader::GetMagicParameter() {
	return m_impl->magicParameter.Get();
}

