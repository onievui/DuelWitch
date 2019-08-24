#pragma once
#ifndef AI_CAST_MAGIC_COMMAND_DEFINED
#define AI_CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


/// <summary>
/// AI魔法詠唱コマンドクラス
/// </summary>
class AICastMagicCommand : public Command {
public:
	// AI魔法詠唱コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 次の攻撃までの待ち時間
	float m_waitTime;

private:
	/// <summary>
	/// AI魔法詠唱コマンドクラス用パラメータ
	/// </summary>
	class AICastMagicCommandData {
	public:
		// データを読み込む
		bool Load();

	public:
		float shotableAngle;
		float castDelay;
	};

	// 読み込みデータ保持オブジェクト
	static LoadDataHolder<AICastMagicCommandData, LoadDataID::PlayScene> s_data;

};


#endif // !AI_CAST_MAGIC_COMMAND_DEFINED
