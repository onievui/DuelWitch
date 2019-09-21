#pragma once
#ifndef USER_RENDER_COMMAND_DEFINED
#define USER_RENDER_COMMAND_DEFINED


#include "RenderCommand.h"


/// <summary>
/// ���[�U�`��R�}���h�N���X
/// </summary>
class UserRenderCommand : public RenderCommand {
public:
	// ���[�U�`��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;
	// �v���C���[��`�悷��
	void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const override;

private:
	// �^�[�Q�b�g�̕����������A�C�R���̈ʒu���v�Z����
	DirectX::SimpleMath::Vector2 CalculateIconPos(const DirectX::SimpleMath::Vector3& vec, const DirectX::SimpleMath::Vector2& screenSize,
		const DirectX::SimpleMath::Vector2& screenOffset);
	// ����v���C���[�̃A�C�R����`�悷��
	void RenderEnemeyIcon(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// �����G�������g��`�悷��
	void RenderElements(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// HP�o�[��`�悷��
	void RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// SP�o�[��`�悷��
	void RenderSpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const;
	// �Ə���`�悷��
	void RenderAiming(const Player& player, DirectX::SpriteBatch* spriteBatch) const;

private:
	// �^�[�Q�b�g�̕����������A�C�R���̈ʒu
	DirectX::SimpleMath::Vector2 m_targetIconPos;
	// �^�[�Q�b�g�̕����������A�C�R����`�悷�邩�ǂ���
	bool                         m_enableRenderTargetIcon;
};


#endif // !USER_RENDER_COMMAND_DEFINED
