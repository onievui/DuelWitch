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

private:
	// ターゲットの方向を示すアイコンの位置を計算する
	DirectX::SimpleMath::Vector2 CalculateIconPos(const DirectX::SimpleMath::Vector3& vec, const DirectX::SimpleMath::Vector2& screenSize,
		const DirectX::SimpleMath::Vector2& screenOffset);

private:
	// ターゲットの方向を示すアイコンの位置
	DirectX::SimpleMath::Vector2 m_targetIconPos;
	// ターゲットの方向を示すアイコンを描画するかどうか
	bool                         m_enableRenderTargetIcon;
};


#endif // !USER_RENDER_COMMAND_DEFINED
