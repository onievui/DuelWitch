#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


class PlayerTrailEffectEmitter;


/// <summary>
/// AI移動コマンドクラス
/// </summary>
class AIMoveCommand : public Command {
public:
	// コンストラクタ
	AIMoveCommand();

public:
	// AI移動コマンドを初期化する
	void Initialize(Player& player) override;
	// AI移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// オイラー角
	DirectX::SimpleMath::Vector3            m_euler;
	//　プレイヤーの軌跡エフェクトへのポインタ
	PlayerTrailEffectEmitter*               m_pEffect;
	// エフェクト用の姿勢クラス
	ChildTransform                          m_effectTransform;
};


#endif // !AI_MOVE_COMMAND_DEFINED
