#pragma once
#ifndef IEFFECT_EMITTER_DEFINED
#define IEFFECT_EMITTER_DEFINED


#include <Framework\StepTimer.h>


class Camera;


/// <summary>
/// �G�t�F�N�g�G�~�b�^�[�C���^�t�F�[�X�N���X
/// </summary>
class IEffectEmitter {
public:
	// �f�X�g���N�^
	virtual ~IEffectEmitter() = default;

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	virtual void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) = 0;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	virtual void Update(const DX::StepTimer& timer, const Camera* camera) = 0;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;

public:
	// �G�t�F�N�g���g�p���Ă��邩�ǂ����擾����
	virtual bool IsUsed() const = 0;
	// �G�t�F�N�g���g�p���邩�ǂ����ݒ肷��
	virtual void IsUsed(bool isUsed) = 0;

};


#endif // !IEFFECT_EMITTER_DEFINED