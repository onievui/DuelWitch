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
	// AI魔法詠唱コマンドを処理するState::
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 最も近い敵プレイヤーを取得する
	const Player* GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* distance);

private:
	// 次の攻撃までの待ち時間
	float m_waitTime;

};


#endif // !AI_CAST_MAGIC_COMMAND_DEFINED
