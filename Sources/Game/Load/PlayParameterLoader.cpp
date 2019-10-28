#include "PlayParameterLoader.h"
#include <Utils\LoadDataManager.h>
#include <Utils\LoadDataHolder.h>

#include <Parameters\CommandParameter.h>
#include <Parameters\CharaStatusParameter.h>
#include <Parameters\FieldParameter.h>
#include <Parameters\ElementParameter.h>
#include <Parameters\MagicParameter.h>
#include <Parameters\EffectParameter.h>


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
	LoadDataHolder<CommandParameter,     LoadDataID::PlayScene> commandParameter;
	LoadDataHolder<CharaStatusParameter, LoadDataID::PlayScene> charaStatusParameter;
	LoadDataHolder<FieldParameter,       LoadDataID::PlayScene> fieldParameter;
	LoadDataHolder<ElementParameter,     LoadDataID::PlayScene> elementParameter;
	LoadDataHolder<MagicParameter,       LoadDataID::PlayScene> magicParameter;
	LoadDataHolder<EffectParameter,      LoadDataID::PlayScene> effectParameter;

};

/// <summary>
/// コンストラクタ
/// </summary>
PlayParameterLoader::Impl::Impl() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Register(&commandParameter);
	manager->Register(&charaStatusParameter);
	manager->Register(&fieldParameter);
	manager->Register(&elementParameter);
	manager->Register(&magicParameter); 
	manager->Register(&effectParameter);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayParameterLoader::Impl::~Impl(){
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Unregister(&commandParameter);
	manager->Unregister(&charaStatusParameter);
	manager->Unregister(&fieldParameter);
	manager->Unregister(&elementParameter);
	manager->Unregister(&effectParameter);
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
const CharaStatusParameter * PlayParameterLoader::GetCharaStatusParameter() {
	return m_impl->charaStatusParameter.Get();
}

/// <summary>
/// パラメータを取得する
/// </summary>
/// <returns>
/// パラメータへのポインタ
/// </returns>
const FieldParameter* PlayParameterLoader::GetFieldParameter() {
	return m_impl->fieldParameter.Get();
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

/// <summary>
/// パラメータを取得する
/// </summary>
/// <returns>
/// パラメータへのポインタ
/// </returns>
const EffectParameter * PlayParameterLoader::GetEffectParameter() {
	return m_impl->effectParameter.Get();
}

