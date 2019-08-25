#pragma once
#ifndef ILOAD_DATA_DEFINED
#define ILOAD_DATA_DEFINED


/// <summary>
/// 読み込みデータインタフェースクラス
/// </summary>
class ILoadData {
public:
	// データを読み込む
	virtual bool Load() = 0;
	// データを再読み込みする
	virtual bool Reload() = 0;
	// データを保存する
	virtual bool Save() = 0;
};

#endif // !ILOAD_DATA_DEFINED
