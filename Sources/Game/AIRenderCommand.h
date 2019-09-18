#pragma once
#ifndef AI_RENDER_COMMAND_DEFINED
#define AI_RENDER_COMMAND_DEFINED


#include "RenderCommand.h"


/// <summary>
/// AI描画コマンドクラス
/// </summary>
class AIRenderCommand : public RenderCommand {
public:
	// AI描画コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;
	// プレイヤーを描画する
	void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const override;
};


#endif // !AI_RENDER_COMMAND_DEFINED
