#pragma once
#ifndef USER_RENDER_COMMAND_DEFINED
#define USER_RENDER_COMMAND_DEFINED


#include "RenderCommand.h"


/// <summary>
/// ���[�U�`��R�}���h�N���X
/// </summary>
class UserRenderCommand : public RenderCommand {
public:
	// ���[�U�`��R�}���h������������
	void Initialize(Player& player) override;
	// ���[�U�`��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;
	// �v���C���[��`�悷��
	void Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const override;

private:
	// �^�[�Q�b�g�̕����������A�C�R�����X�V����
	void UpdateIcon(Player& player);
	// �^�[�Q�b�g�̕����������A�C�R���̈ʒu���v�Z����
	DirectX::SimpleMath::Vector2 CalculateIconPos(const DirectX::SimpleMath::Vector3& vec, const DirectX::SimpleMath::Vector2& screenSize,
		const DirectX::SimpleMath::Vector2& screenOffset);
	// �Ə����X�V����
	void UpdateAiming(Player& player, const DX::StepTimer& timer);

private:
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
	// �^�[�Q�b�g�̕����������A�C�R���Ɋւ�����
	struct TargetIconInfo {
		// �ʒu
		DirectX::SimpleMath::Vector2 pos;
		// �`�悷�邩�ǂ���
		bool                         enable;
		// �L����ID
		int                          charaId;
	};

private:
	// �^�[�Q�b�g�̕����������A�C�R���Ɋւ�����
	std::vector<TargetIconInfo>                    m_targetIconInfo;
	// �Ə��̈ʒu
	DirectX::SimpleMath::Vector2                   m_aimingPos;
	// �Ə��̃A�j���[�V�����p�J�E���^
	float                                          m_aimigRotation;
};


#endif // !USER_RENDER_COMMAND_DEFINED
