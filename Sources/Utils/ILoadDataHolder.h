#pragma once
#ifndef ILOAD_DATA_HOLDER_DEFINED
#define ILOAD_DATA_HOLDER_DEFINED


#include "LoadDataID.h"


/// <summary>
/// 読み込みデータ保持インタフェースクラス
/// </summary>
class ILoadDataHolder {
public:
	// データを読み込む
	virtual bool Load() = 0;
	// データを再読み込みする
	virtual bool Reload() = 0;
	// データを開放する
	virtual void Dispose() = 0;
	// 読み込みデータIDを取得する
	virtual LoadDataID GetID() const = 0;

};


#endif // !ILOAD_DATA_HOLDER_DEFINED
