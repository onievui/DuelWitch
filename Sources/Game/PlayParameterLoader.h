#pragma once
#ifndef PLAY_PARAMETER_LOADER_DEFINED
#define PLAY_PARAMETER_LOADER_DEFINED


#include "IParameterLoader.h"
#include <Utils\LoadDataHolder.h>

#include <Parameters\CommandParameter.h>
#include <Parameters\AICommandParameter.h>
#include <Parameters\ElementParameter.h>


/// <summary>
/// プレイシーン用パラメータ読み込みクラス
/// </summary>
class PlayParameterLoader : public IParameterLoader {
public:
	// コンストラクタ
	PlayParameterLoader();
	// デストラクタ
	~PlayParameterLoader();

public:
	// パラメータを読み込む
	void Load() override;
	// パラメータを再読み込みする
	void Reload() override;
	// パラメータを開放する
	void Dispose() override;

public:
	const CommandParameter*   GetCommandParameter()   { return m_commandParameter.Get(); }
	const AICommandParameter* GetAICommandParameter() { return m_aiCommandParameter.Get(); }
	const ElementParameter*   GetElementParameter()   { return m_elementParameter.Get(); }

private:
	LoadDataHolder<CommandParameter, LoadDataID::PlayScene>   m_commandParameter;
	LoadDataHolder<AICommandParameter, LoadDataID::PlayScene> m_aiCommandParameter;
	LoadDataHolder<ElementParameter, LoadDataID::PlayScene>   m_elementParameter;

};

#endif // !PLAY_PARAMETER_LOADER_DEFINED
