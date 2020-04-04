#pragma once
#ifndef EFFECTIVE_EFFECT_EMITTER_DEFINED
#define EFFECTIVE_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class EffectiveEffect;
enum class MagicID;


/// <summary>
/// �ł������E���˃G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class EffectiveEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	EffectiveEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// ���@ID�ŃG�t�F�N�g�̐F��ݒ肷��
	void SetColorID(MagicID id);

private:
	// �G�t�F�N�g
	std::vector<std::unique_ptr<EffectiveEffect>> m_effects;
	// �����x�N�g��
	DirectX::SimpleMath::Vector3                  m_eyeVec;
	// �F
	DirectX::SimpleMath::Color                    m_color;

};


#endif // !EFFECTIVE_EFFECT_EMITTER_DEFINED
