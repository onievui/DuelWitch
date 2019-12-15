#pragma once
#ifndef PLAYER_CHARGE_EFFECT_EMITTER_DEFINED
#define PLAYER_CHARGE_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class PlayerChargeEffect;


/// <summary>
/// �v���C���[�̃`���[�W�G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class PlayerChargeEffectEmitter : public EffectEmitter {
public:
	// �`���[�W���
	enum class State {
		Charge0,	// 0�i�K
		Charge1,	// 1�i�K
		Charge2,	// 2�i�K

		None,		// �`���[�W���Ă��Ȃ�
	};

public:
	// �R���X�g���N�^
	PlayerChargeEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// �e�I�u�W�F�N�g��ݒ肷��
	void SetParent(const Transform* pParent) override;
	// �`���[�W��Ԃ�ݒ肷��
	void SetChargeState(State state) { m_chargeState = state; }

private:
	// �F���擾����
	DirectX::SimpleMath::Color GetColor() const;

private:
	// �G�t�F�N�g
	std::vector<std::unique_ptr<PlayerChargeEffect>> m_effects;
	// �����x�N�g��
	DirectX::SimpleMath::Vector3                     m_eyeVec;
	// �`���[�W���
	State                                            m_chargeState;

};


#endif // !PLAYER_CHARGE_EFFECT_EMITTER_DEFINED
