#pragma once
#ifndef RENDER_COMMAND_DEFINED
#define RENDER_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// �`��R�}���h�N���X
/// </summary>
class RenderCommand : public Command {
public:
	// ��HP������������܂ł̎���
	static constexpr float RED_HP_TIME = 1.0f;
	// ��HP�̌������x
	static constexpr float RED_HP_SPEED = 15.0f;

public:
	virtual ~RenderCommand() = default;

public:
	// �v���C���[��`�悷��
	virtual void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
		player, view, proj, spriteBatch;
	};

public:
	// ��HP���X�V����
	void UpdateRedHpBar(Player& player, const DX::StepTimer& timer);

public:
	// �v���C���[�̃��f����`�悷��
	void RenderPlayerModel(const Player& player, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

protected:
	// �O���HP
	float                                          m_preHp;
	// �ԕ\���pHP
	float                                          m_redHp;
	// ��HP�����p�^�C�}�[
	float                                          m_redHpTime;

};


#endif // !RENDER_COMMAND_DEFINED
