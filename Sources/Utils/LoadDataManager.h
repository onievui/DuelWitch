#pragma once
#ifndef LOAD_DATA_MANAGER_DEFINED
#define LOAD_DATA_MANAGER_DEFINED


#include "Singleton.h"


class LoadData;


/// <summary>
/// 読み込みデータマネージャクラス
/// </summary>
class LoadDataManager : public Singleton<LoadDataManager> {
	friend class Singleton<LoadDataManager>;
public:
	// コンストラクタ
	LoadDataManager();
	// デストラクタ
	~LoadDataManager();

public:
	// データの登録
	void Regiser(LoadData* loadData);
	// データを読み込む
	void Load();

private:
	// 読み込み対象のデータ
	std::vector<LoadData*> m_loadData;

};


#endif // !LOAD_DATA_MANAGER_DEFINED
