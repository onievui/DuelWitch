#pragma once
#ifndef FIRE_MAGIC_EFFECT_EMITTER_DEFINED
#define FIRE_MAGIC_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


/// <summary>
/// �����@�G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class FireMagicEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	FireMagicEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:

};


#endif // !FIRE_MAGIC_EFFECT_EMITTER_DEFINED