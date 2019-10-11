#pragma once
#ifndef AI_RENDER_COMMAND_DEFINED
#define AI_RENDER_COMMAND_DEFINED


#include "RenderCommand.h"


/// <summary>
/// AI描画コマンドクラス
/// </summary>
class AIRenderCommand : public RenderCommand {
public:
	// AI描画コマンドを初期化する
	void Initialize(Player& player) override;
	// AI描画コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;
	// プレイヤーを描画する
	void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const override;

private:
	// HPバーを描画する
	void RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const;

private:
	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect>                                m_batchEffect;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                m_inputLayout;

};


#endif // !AI_RENDER_COMMAND_DEFINED
