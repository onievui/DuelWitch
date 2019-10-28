#pragma once
#ifndef IPARAMETER_LOADER_DEFINED
#define IPARAMETER_LOADER_DEFINED


/// <summary>
/// パラメータ読み込みインタフェースクラス
/// </summary>
class IParameterLoader {
public:
	// パラメータを読み込む
	virtual void Load() = 0;
	// パラメータを再読み込みする
	virtual void Reload() = 0;
	// パラメータを開放する
	virtual void Dispose() = 0;
};


#endif // !IPARAMETER_LOADER_DEFINED
