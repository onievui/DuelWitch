#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include "EmptyObject.h"


class PlayerTrailEffectEmitter;


/// <summary>
/// 移動コマンドクラス
/// </summary>
class MoveCommand : public Command {
public:
	// コンストラクタ
	MoveCommand();

public:
	// 移動コマンドを初期化する
	void Initialize(Player& player) override;
	// 移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 移動によるズーム
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);

private:
	// カメラターゲット
	EmptyObject                             m_cameraTarget;
	// 継続ブースト時間
	float                                   m_boostTime;
	// デフォルトの画角
	float                                   m_defaultFov;
	// オイラー角
	DirectX::SimpleMath::Vector3            m_euler;
	//　プレイヤーの軌跡エフェクトへのポインタ
	PlayerTrailEffectEmitter*               m_pEffect;
	// エフェクト用の姿勢クラス
	ChildTransform                          m_effectTransform;
};


#endif // !MOVE_COMMAND_DEFINED
