#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadData.h>


/// <summary>
/// 移動コマンドクラス
/// </summary>
class MoveCommand : public Command {
public:
	// コンストラクタ
	MoveCommand() : m_totalElapsedTime(), m_euler() {}

public:
	// 移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 総経過時間
	float                        m_totalElapsedTime;
	// オイラー角
	DirectX::SimpleMath::Vector3 m_euler;

private:
	/// <summary>
	/// 移動コマンドクラスデータ
	/// </summary>
	class MoveCommandData : public LoadData {
	public:
		// コンストラクタ
		MoveCommandData() : LoadData(LoadDataID::PlayScene), m_value() {}

	public:
		// データを読み込む
		bool Load() override;
		void Dispose() override;

	private:
		// 読み込みデータ本体
		struct _value {
			float moveSpeed;
			float moveSpeedXY;
			float rotSpeed;
			float rotZLimit;
			float rotXLimit;
			float rotYLimit;
			float lerpSpeed;
		};

	public:
		_value* operator->() {
			return m_value.get();
		}

	private:
		// 読み込みデータ本体
		std::unique_ptr<_value> m_value;

	};

	// 読み込みデータ
	static MoveCommandData s_data;
};


#endif // !MOVE_COMMAND_DEFINED
