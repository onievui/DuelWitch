#pragma once
#ifndef EFFECT_MANAGER_DEFINED
#define EFFECT_MANAGER_DEFINED


#include <Framework\StepTimer.h>


class IEffectEmitter;
class EffectFactory;
enum class EffectID;
class Camera;

/// <summary>
/// �G�t�F�N�g�}�l�[�W���N���X
/// </summary>
class EffectManager {
public:
	// �R���X�g���N�^
	EffectManager();
	// �f�X�g���N�^
	~EffectManager();

public:
	// �G�t�F�N�g�}�l�[�W��������������
	void Initialize();
	// �G�t�F�N�g�}�l�[�W�����X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera);
	// �G�t�F�N�g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// �G�t�F�N�g�𐶐�����
	void CreateEffect(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);

private:
	// �����ς݃G�t�F�N�g
	std::vector<IEffectEmitter*>                    m_effects;
	// �G�t�F�N�g�t�@�N�g��
	std::unique_ptr<EffectFactory>                  m_effectFactory;

};


#endif // !EFFECT_MANAGER_DEFINED
