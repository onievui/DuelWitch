#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


class Element;
class PlayerTrailEffectEmitter;


/// <summary>
/// AI移動コマンドクラス
/// </summary>
class AIMoveCommand : public Command {
public:
	// コンストラクタ
	AIMoveCommand();
	// デストラクタ
	~AIMoveCommand();

public:
	// AI移動コマンドを初期化する
	void Initialize(Player& player) override;
	// AI移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// AIの処理を行う
	void ExecuteAI(Player& player);
	// 最も近い敵プレイヤーを取得する
	const Player* GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* distance);
	// 敵プレイヤーの方を向いているか調べる
	bool IsLookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos);
	// 敵プレイヤーへの向き具合を調べる
	float LookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos);
	// 最も取りやすいエレメントを取得する
	const Element* GetTargetElement(const Transform& transform);
	// ベクトルの左右判定をする
	float CheckVecX(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir);
	// ベクトルの上下判定をする
	float CheckVecY(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir);
	// エレメント収集の処理を行う
	void ExecuteCollectElement(Player& player);
	// 敵プレイヤーの追跡の処理を行う
	void ExecuteChase(Player& player);
	// 敵プレイヤーからの逃避の処理を行う
	void ExecuteEvade(Player& player);

private:
	// AI移動コマンド用ステート
	enum class MoveState {
		// エレメントを収集する
		CollectElement,
		// 敵プレイヤーを追いかける
		Chase,
		// 敵プレイヤーから逃げる
		Evade,
	};
	
	// 行動の情報構造体
	struct MoveInfo {
		// X軸の移動方向
		float xVec;
		// Y軸の移動方向
		float yVec;
		// ブースト移動をするかどうか
		bool useBoost;
	};

private:
	// 現在のステート
	MoveState                               m_state;
	// 行動の情報
	MoveInfo                                m_moveInfo;
	// オイラー角
	DirectX::SimpleMath::Vector3            m_euler;
	//　プレイヤーの軌跡エフェクトへのポインタ
	PlayerTrailEffectEmitter*               m_pEffect;
	// エフェクト用の姿勢クラス
	ChildTransform                          m_effectTransform;
};


#endif // !AI_MOVE_COMMAND_DEFINED
