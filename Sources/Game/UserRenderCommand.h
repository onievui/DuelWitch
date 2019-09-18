#pragma once
#ifndef USER_RENDER_COMMAND_DEFINED
#define USER_RENDER_COMMAND_DEFINED


#include "RenderCommand.h"


/// <summary>
/// ユーザ描画コマンドクラス
/// </summary>
class UserRenderCommand : public RenderCommand {
public:
	// ユーザ描画コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;
	// プレイヤーを描画する
	void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const override;
};


#endif // !USER_RENDER_COMMAND_DEFINED
