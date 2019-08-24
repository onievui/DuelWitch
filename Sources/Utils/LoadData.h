#pragma once
#ifndef LOAD_DATA_DEFINED
#define LOAD_DATA_DEFINED


#include "LoadDataID.h"


/// <summary>
/// 読み込みデータクラス
/// </summary>
class LoadData {
public:
	// コンストラクタ
	LoadData(LoadDataID id);
	// デストラクタ
	virtual ~LoadData() = default;

public:
	// データを読み込む
	virtual bool Load() = 0;
	// データを開放する
	virtual void Dispose() = 0;

public:
	// ID
	const LoadDataID m_id;
	
protected:
	// 読み込みデータ本体（派生クラスで定義する）
	// std::unique_ptr<_value> m_value;

};


#endif // !LOAD_DATA_DEFINED