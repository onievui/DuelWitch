#pragma once
#ifndef AI_RENDER_COMMAND_DEFINED
#define AI_RENDER_COMMAND_DEFINED


#include "RenderCommand.h"


/// <summary>
/// AI�`��R�}���h�N���X
/// </summary>
class AIRenderCommand : public RenderCommand {
public:
	// AI�`��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;
	// �v���C���[��`�悷��
	void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const override;
};


#endif // !AI_RENDER_COMMAND_DEFINED
