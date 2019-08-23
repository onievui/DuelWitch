#pragma once
#ifndef LOAD_DATA_DEFINED
#define LOAD_DATA_DEFINED


/// <summary>
/// 読み込みデータ
/// </summary>
class LoadData {
public:
	// コンストラクタ
	LoadData();
	// デストラクタ
	virtual ~LoadData() = default;

public:
	// データを読み込む
	virtual bool Load() = 0;

};


#endif // !LOAD_DATA_DEFINED