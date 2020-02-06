#pragma once
#ifndef RENDER_COMMAND_DEFINED
#define RENDER_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// 描画コマンドクラス
/// </summary>
class RenderCommand : public Command {
public:
	// 赤HPを減少させるまでの時間
	static constexpr float RED_HP_TIME = 1.0f;
	// 赤HPの減少速度
	static constexpr float RED_HP_SPEED = 15.0f;

public:
	virtual ~RenderCommand() = default;

public:
	// プレイヤーを描画する
	virtual void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
		player, view, proj, spriteBatch;
	};

public:
	// 赤HPを更新する
	void UpdateRedHpBar(Player& player, const DX::StepTimer& timer);

public:
	// プレイヤーのモデルを描画する
	void RenderPlayerModel(const Player& player, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

protected:
	// 前回のHP
	float                                          m_preHp;
	// 赤表示用HP
	float                                          m_redHp;
	// 赤HP減少用タイマー
	float                                          m_redHpTime;

};


#endif // !RENDER_COMMAND_DEFINED
