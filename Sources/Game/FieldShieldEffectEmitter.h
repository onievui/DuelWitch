#pragma once
#ifndef FIELD_SHIELD_EFFECT_EMITTER_DEFINED
#define FIELD_SHIELD_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class FieldShieldEffect;


/// <summary>
/// �t�B�[���h�Փ˃G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class FieldShieldEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	FieldShieldEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// �G�t�F�N�g��ǉ�����
	void AddEffect(const DirectX::SimpleMath::Vector3& pos);

private:
	// �G�t�F�N�g
	std::vector<std::unique_ptr<FieldShieldEffect>> m_effects;

};


#endif // !FIELD_SHIELD_EFFECT_EMITTER_DEFINED
