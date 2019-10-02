#pragma once
#ifndef CAST_MAGIC_COMMAND_DEFINED
#define CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// 魔法詠唱コマンドクラス
/// </summary>
class CastMagicCommand : public Command {
public:
	// コンストラクタ
	CastMagicCommand();

public:
	// 魔法詠唱コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 通常状態の処理を行う
	void ExecuteIdel(Player& player, const DX::StepTimer& timer);
	// チャージ中の処理を行う
	void ExecuteCharging(Player& player, const DX::StepTimer& timer);
	// チャージショットが出来る段階を調べる
	int ChargeAllowedLevel(const std::list<ElementID>& elements);

	// 魔法のためのレイ用平面の作成
	DirectX::SimpleMath::Plane CreatePlaneForMagic(const Transform& transform);

private:
	// チャージショット用ステート
	enum class ChargeState {
		Idel,		// 通常状態
		Charging	// チャージ中
	};

private:
	// 現在のステート
	ChargeState          m_state;
	// チャージ時間
	float                m_chargingTime;
	// チャージ可能な段階
	int                  m_chargeAllowedLevel;
};


#endif // !CAST_MAGIC_COMMAND_DEFINED
