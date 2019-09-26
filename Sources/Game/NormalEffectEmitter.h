#pragma once
#ifndef NORMAL_MAGIC_EFFECT_EMITTER_DEFINED
#define NORMAL_MAGIC_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


/// <summary>
/// �ʏ햂�@�G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class NormalMagicEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	NormalMagicEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:
	// �����x�N�g��
	DirectX::SimpleMath::Vector3 m_eyeVec;
};


#endif // !NORMAL_MAGIC_EFFECT_EMITTER_DEFINED
