#pragma once
#ifndef LOAD_DATA_MANAGER_DEFINED
#define LOAD_DATA_MANAGER_DEFINED


#include "Singleton.h"
#include "LoadDataID.h"


class ILoadDataHolder;


/// <summary>
/// 読み込みデータマネージャクラス
/// </summary>
class LoadDataManager : public SingletonWithFunc<LoadDataManager> {
	friend class SingletonWithFunc<LoadDataManager>;

public:
	// コンストラクタ
	LoadDataManager();
	// デストラクタ
	~LoadDataManager();

public:
	// データの登録
	void Register(ILoadDataHolder* loadDataHolder);
	// データの解除
	void Unregister(ILoadDataHolder* loadDataHolder);
	// データを読み込む
	void Load(LoadDataID id);
	// データを再読み込みする
	void Reload(LoadDataID id);
	// データを開放する
	void Dispose(LoadDataID id);

private:
	// 読み込み対象のデータ
	std::vector<ILoadDataHolder*> m_loadDataHolder;

};


#endif // !LOAD_DATA_MANAGER_DEFINED
