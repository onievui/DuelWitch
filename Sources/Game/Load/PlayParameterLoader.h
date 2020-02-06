#pragma once
#ifndef PLAY_PARAMETER_LOADER_DEFINED
#define PLAY_PARAMETER_LOADER_DEFINED


#include "IParameterLoader.h"


class CommandParameter;
class CharaStatusParameter;
class FieldParameter;
class ElementParameter;
class MagicParameter;
class EffectParameter;
class FuzzyParameter;
class OptionParameter;


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
	// パラメータを取得する
	const CommandParameter*     GetCommandParameter();
	const CharaStatusParameter* GetCharaStatusParameter();
	const FieldParameter*       GetFieldParameter();
	const ElementParameter*     GetElementParameter();
	const MagicParameter*       GetMagicParameter();
	const EffectParameter*      GetEffectParameter();
	const FuzzyParameter*       GetFuzzyParameter();
	const OptionParameter*      GetOptionParameter();

private:
	// ヘッダファイルでのインクルードを減らすためのクラス
	class Impl;
	std::unique_ptr<PlayParameterLoader::Impl> m_impl;

};

#endif // !PLAY_PARAMETER_LOADER_DEFINED
