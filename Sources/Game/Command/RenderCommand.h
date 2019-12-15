#pragma once
#ifndef RENDER_COMMAND_DEFINED
#define RENDER_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// 描画コマンドクラス
/// </summary>
class RenderCommand : public Command {
public:
	virtual ~RenderCommand() = default;

public:
	// プレイヤーを描画する
	virtual void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
		player, view, proj, spriteBatch;
	};

public:
	// プレイヤーのモデルを描画する
	void RenderPlayerModel(const Player& player, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

};


#endif // !RENDER_COMMAND_DEFINED
