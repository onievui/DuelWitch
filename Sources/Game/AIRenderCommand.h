#pragma once
#ifndef AI_RENDER_COMMAND_DEFINED
#define AI_RENDER_COMMAND_DEFINED


#include "RenderCommand.h"


/// <summary>
/// AI�`��R�}���h�N���X
/// </summary>
class AIRenderCommand : public RenderCommand {
public:
	// AI�`��R�}���h������������
	void Initialize(Player& player) override;
	// AI�`��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;
	// �v���C���[��`�悷��
	void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const override;

private:
	// HP�o�[��`�悷��
	void RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const;

private:
	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect>                                m_batchEffect;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                m_inputLayout;

};


#endif // !AI_RENDER_COMMAND_DEFINED
