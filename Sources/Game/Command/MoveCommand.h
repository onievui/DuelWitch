#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include <Game\Common\EmptyObject.h>


class PlayerTrailEffectEmitter;
class TargetCamera;


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
	// 通常移動の処理を行う
	void ExcuteMove(Player& player, const DX::StepTimer& timer);
	// ロール回避の処理を行う
	void ExcuteRoll(Player& player, const DX::StepTimer& timer);
	// ロール回避の入力判定
	void RollInputCheck(Player& player, const DX::StepTimer& timer);
	// 移動によるズーム
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);
	// 照準によるカメラ向きを調整する
	void AdjustCamera(TargetCamera* targetCamera);

private:
	// 移動コマンド用ステート
	enum class MoveState {
		Move,	// 通常移動
		Roll,	// ロール回避
	};

	// ロール回避に関する情報
	struct RollInfo {
		// 左にロールする時の入力猶予時間
		float                           leftGraceTime;
		// 右にロールする時の入力猶予時間
		float                           rightGraceTime;
		// ロールしてから経過時間
		float                           rollingTime;
		// ロールしている方向
		bool                            isRollingLeft;
		// ロール前のZ軸回転量
		float                           preRotZ;
	};

private:
	// 現在のステート
	MoveState                               m_state;
	// ロールに関する情報
	RollInfo                                m_rollInfo;
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
