#pragma once
#ifndef PLAYER_TRAIL_EFFECT_EMITTER_DEFINED
#define PLAYER_TRAIL_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class PlayerTrailEffect;


/// <summary>
/// �v���C���[�̋O���G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class PlayerTrailEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	PlayerTrailEffectEmitter();

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

private:
	// �G�t�F�N�g
	std::vector<std::unique_ptr<PlayerTrailEffect>> m_effects;
	// �����x�N�g��
	DirectX::SimpleMath::Vector3                    m_eyeVec;
	// �����̔��˕���
	DirectX::SimpleMath::Vector3                    m_createDir;
};


#endif // !PLAYER_TRAIL_EFFECT_EMITTER_DEFINED
