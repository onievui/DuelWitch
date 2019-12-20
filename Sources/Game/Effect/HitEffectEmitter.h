#pragma once
#ifndef HIT_EFFECT_EMITTER_DEFINED
#define HIT_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class HitEffect;


/// <summary>
/// �q�b�g�G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class HitEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	HitEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:
	// �G�t�F�N�g
	std::vector<std::unique_ptr<HitEffect>> m_effects;
	// �����x�N�g��
	DirectX::SimpleMath::Vector3 m_eyeVec;
};


#endif // !HIT_EFFECT_EMITTER_DEFINED
