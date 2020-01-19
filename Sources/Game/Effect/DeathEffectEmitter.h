#pragma once
#ifndef DEATH_EFFECT_EMITTER_DEFINED
#define DEATH_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class DeathEffect;


/// <summary>
/// ���j�G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class DeathEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	DeathEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// �G�t�F�N�g�̐F�ԍ���ݒ肷��
	void SetColorID(int id);

private:
	// �G�t�F�N�g
	std::vector<std::unique_ptr<DeathEffect>> m_effects;
	// �����x�N�g��
	DirectX::SimpleMath::Vector3              m_eyeVec;
	// �F
	DirectX::SimpleMath::Color                m_color;
};


#endif // !DEATH_EFFECT_EMITTER_DEFINED
