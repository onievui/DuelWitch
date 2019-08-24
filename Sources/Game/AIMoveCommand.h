#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


/// <summary>
/// AI移動コマンドクラス
/// </summary>
class AIMoveCommand : public Command {
public:
	// コンストラクタ
	AIMoveCommand() : m_totalElapsedTime(), m_euler() {}

public:
	// AI移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 総経過時間
	float                        m_totalElapsedTime;
	// オイラー角
	DirectX::SimpleMath::Vector3 m_euler;

private:
	/// <summary>
	/// AI移動コマンドクラス用パラメータ
	/// </summary>
	class AIMoveCommandData {
	public:
		// データを読み込む
		bool Load();

	public:
		float moveSpeed;
		float moveSpeedXY;
		float rotSpeed;
		float rotZLimit;
		float rotXLimit;
		float rotYLimit;
		float lerpSpeed;
		float nearDistance;
	};

	// 読み込みデータ保持オブジェクト
	static LoadDataHolder<AIMoveCommandData, LoadDataID::PlayScene> s_data;
};


#endif // !AI_MOVE_COMMAND_DEFINED
