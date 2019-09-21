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
	// 相手プレイヤーのアイコンを描画する
	void RenderEnemeyIcon(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// 所持エレメントを描画する
	void RenderElements(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// HPバーを描画する
	void RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// SPバーを描画する
	void RenderSpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// 照準を描画する
	void RenderAiming(const Player& player, DirectX::SpriteBatch* spriteBatch) const;

private:
	// ターゲットの方向を示すアイコンの位置
	DirectX::SimpleMath::Vector2 m_targetIconPos;
	// ターゲットの方向を示すアイコンを描画するかどうか
	bool                         m_enableRenderTargetIcon;
};


#endif // !USER_RENDER_COMMAND_DEFINED
