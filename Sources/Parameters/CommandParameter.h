#pragma once
#ifndef COMMAND_PARAMETER_DEFINED
#define COMMAND_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// コマンドクラス用パラメータ
/// </summary>
class CommandParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

public:
	float moveSpeed;
	float moveSpeedXY;
	float rotSpeed;
	float rotZLimit;
	float rotXLimit;
	float rotYLimit;
	float lerpSpeed;
};

#endif // !COMMAND_PARAMETER_DEFINED
