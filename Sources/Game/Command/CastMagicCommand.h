#pragma once
#ifndef CAST_MAGIC_COMMAND_DEFINED
#define CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// 魔法詠唱コマンドクラス
/// </summary>
class CastMagicCommand : public Command {
public:
	// 右スティックの感度
	static constexpr float CURSOR_SENSITIVITY = 1280.0f;
	// チャージ効果音を鳴らす間隔
	static constexpr float CHARGING_SOUND_DELAY_TIME = 1.0f;

public:
	// コンストラクタ
	CastMagicCommand();

public:
	// 魔法詠唱コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 通常状態の処理を行う
	void ExecuteIdle(Player& player, const DX::StepTimer& timer);
	// チャージ中の処理を行う
	void ExecuteCharging(Player& player, const DX::StepTimer& timer);
	// チャージショットが出来る段階を調べる
	int ChargeAllowedLevel(const std::list<ElementID>& elements);
	// 照準を操作する
	void ControlAim(Player& player, const DX::StepTimer& timer);
	// 照準に敵プレイヤーが重なっているか判定する
	bool LockOnOtherPlayer(Player& player);
	// 魔法のためのレイ用平面の作成
	DirectX::SimpleMath::Plane CreatePlaneForMagic(const Transform& transform);

private:
	// チャージショット用ステート
	enum class ChargeState {
		Idle,		// 通常状態
		Charging	// チャージ中
	};

private:
	// 現在のステート
	ChargeState              m_state;
	// カメラから照準へのレイ
	DirectX::SimpleMath::Ray m_shotRay;
	// チャージ時間
	float                    m_chargingTime;
	// チャージ可能な段階
	int                      m_chargeAllowedLevel;
	// チャージ効果音用タイマー
	float                    m_chargingSoundTime;
};


#endif // !CAST_MAGIC_COMMAND_DEFINED
