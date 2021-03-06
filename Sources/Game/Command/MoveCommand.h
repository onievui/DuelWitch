#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include <Game\Object\EmptyObject.h>


class PlayerTrailEffectEmitter;
class PlayerChargeEffectEmitter;
class TargetCamera;


/// <summary>
/// 移動コマンドクラス
/// </summary>
class MoveCommand : public Command {
public:
	// コンストラクタ
	MoveCommand();
	// デストラクタ
	~MoveCommand();

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
	// クイックターンの処理を行う
	void ExecuteTurn(Player& player, const DX::StepTimer& timer);
	// ロール回避の入力判定
	void RollInputCheck(Player& player, const DX::StepTimer& timer);
	// クイックターンの入力判定
	void TurnInputCheck(Player& player, const DX::StepTimer& timer);
	// 移動によるズーム
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);
	// 照準によるカメラ向きを調整する
	void AdjustCamera(TargetCamera* targetCamera);
	// 衝突時に向きを調整する
	void AdjustRotation(Player& player, const DX::StepTimer& timer);

private:
	// 移動コマンド用ステート
	enum class MoveState {
		Move,	// 通常移動
		Roll,	// ロール回避
		Turn,   // クイックターン
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

	// クイックターンに関する情報
	struct TurnInfo {
		// クイックターンする時の入力猶予時間
		float                           graceTime;
		// クイックターンしてからの経過時間
		float                           turningTime;
		// 回転する方向
		int                             turnDirection;
		// クイックターン前のX軸回転量
		float                           preRotX;
		// クイックターン前のY軸回転量
		float                           preRotY;
	};

private:
	// 現在のステート
	MoveState                               m_state;
	// ロールに関する情報
	RollInfo                                m_rollInfo;
	// クイックターンに関する情報
	TurnInfo                                m_turnInfo;
	// カメラへのポインタ
	TargetCamera*                           m_pTargetCamera;
	// カメラターゲット
	EmptyObject                             m_cameraTarget;
	// 継続ブースト時間
	float                                   m_boostTime;
	// デフォルトの画角
	float                                   m_defaultFov;
	// オイラー角
	DirectX::SimpleMath::Vector3            m_euler;
	// 前回の座標
	DirectX::SimpleMath::Vector3            m_prePos;
	// 前回の移動量
	DirectX::SimpleMath::Vector3            m_preMove;
	//　プレイヤーの軌跡エフェクトへのポインタ
	PlayerTrailEffectEmitter*               m_pTrailEffect;
	// プレイヤーのチャージエフェクトへのポインタ
	PlayerChargeEffectEmitter*              m_pChargeEffect;
	// エフェクト用の姿勢クラス
	ChildTransform                          m_effectTransform;
};


#endif // !MOVE_COMMAND_DEFINED
