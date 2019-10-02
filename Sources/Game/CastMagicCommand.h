#pragma once
#ifndef CAST_MAGIC_COMMAND_DEFINED
#define CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// ���@�r���R�}���h�N���X
/// </summary>
class CastMagicCommand : public Command {
public:
	// �R���X�g���N�^
	CastMagicCommand();

public:
	// ���@�r���R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// �ʏ��Ԃ̏������s��
	void ExecuteIdel(Player& player, const DX::StepTimer& timer);
	// �`���[�W���̏������s��
	void ExecuteCharging(Player& player, const DX::StepTimer& timer);
	// �`���[�W�V���b�g���o����i�K�𒲂ׂ�
	int ChargeAllowedLevel(const std::list<ElementID>& elements);

	// ���@�̂��߂̃��C�p���ʂ̍쐬
	DirectX::SimpleMath::Plane CreatePlaneForMagic(const Transform& transform);

private:
	// �`���[�W�V���b�g�p�X�e�[�g
	enum class ChargeState {
		Idel,		// �ʏ���
		Charging	// �`���[�W��
	};

private:
	// ���݂̃X�e�[�g
	ChargeState          m_state;
	// �`���[�W����
	float                m_chargingTime;
	// �`���[�W�\�Ȓi�K
	int                  m_chargeAllowedLevel;
};


#endif // !CAST_MAGIC_COMMAND_DEFINED
