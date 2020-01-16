#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


class Element;
class PlayerTrailEffectEmitter;


/// <summary>
/// AI�ړ��R�}���h�N���X
/// </summary>
class AIMoveCommand : public Command {
public:
	// �R���X�g���N�^
	AIMoveCommand();
	// �f�X�g���N�^
	~AIMoveCommand();

public:
	// AI�ړ��R�}���h������������
	void Initialize(Player& player) override;
	// AI�ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// AI�̏������s��
	void ExecuteAI(Player& player);
	// �ł��߂��G�v���C���[���擾����
	const Player* GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* distance);
	// �G�v���C���[�̕��������Ă��邩���ׂ�
	bool IsLookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos);
	// �G�v���C���[�ւ̌�����𒲂ׂ�
	float LookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos);
	// �ł����₷���G�������g���擾����
	const Element* GetTargetElement(const Transform& transform);
	// �x�N�g���̍��E���������
	float CheckVecX(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir);
	// �x�N�g���̏㉺���������
	float CheckVecY(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir);
	// �G�������g���W�̏������s��
	void ExecuteCollectElement(Player& player);
	// �G�v���C���[�̒ǐՂ̏������s��
	void ExecuteChase(Player& player);
	// �G�v���C���[����̓����̏������s��
	void ExecuteEvade(Player& player);

private:
	// AI�ړ��R�}���h�p�X�e�[�g
	enum class MoveState {
		// �G�������g�����W����
		CollectElement,
		// �G�v���C���[��ǂ�������
		Chase,
		// �G�v���C���[���瓦����
		Evade,
	};
	
	// �s���̏��\����
	struct MoveInfo {
		// X���̈ړ�����
		float xVec;
		// Y���̈ړ�����
		float yVec;
		// �u�[�X�g�ړ������邩�ǂ���
		bool useBoost;
	};

private:
	// ���݂̃X�e�[�g
	MoveState                               m_state;
	// �s���̏��
	MoveInfo                                m_moveInfo;
	// �I�C���[�p
	DirectX::SimpleMath::Vector3            m_euler;
	//�@�v���C���[�̋O�ՃG�t�F�N�g�ւ̃|�C���^
	PlayerTrailEffectEmitter*               m_pEffect;
	// �G�t�F�N�g�p�̎p���N���X
	ChildTransform                          m_effectTransform;
};


#endif // !AI_MOVE_COMMAND_DEFINED
