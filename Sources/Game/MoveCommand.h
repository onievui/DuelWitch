#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include "EmptyObject.h"


/// <summary>
/// 移動コマンドクラス
/// </summary>
class MoveCommand : public Command {
public:
	// コンストラクタ
	MoveCommand();

public:
	// 移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 移動によるズーム
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);

private:
	// 総経過時間
	float                        m_totalElapsedTime;
	// 継続ブースト時間
	float                        m_boostTime;
	// デフォルトの画角
	float                        m_defaultFov;
	// オイラー角
	DirectX::SimpleMath::Vector3 m_euler;
	// カメラターゲット
	EmptyObject                  m_cameraTarget;

};


#endif // !MOVE_COMMAND_DEFINED
