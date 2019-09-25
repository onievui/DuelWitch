#pragma once
#ifndef NORMAL_MAGIC_DEFINED
#define NORMAL_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class IEffectEmitter;


/// <summary>
/// �ʏ햂�@�N���X
/// </summary>
class NormalMagic : public Magic {
public:
	// �R���X�g���N�^
	NormalMagic();
	// �f�X�g���N�^
	~NormalMagic();

public:
	// �ʏ햂�@�𐶐�����
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) override;
	// �ʏ햂�@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �ʏ햂�@���J������
	void Lost() override;
	// �ʏ햂�@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	// �ʏ햂�@�̏I���������s��
	void Finalize() override;

public:
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const Collider* collider) override;
	// ���@�Ƃ̏Փˏ���
	void HitMagic(const IMagic* other) override;

private:
	// �ʏ햂�@�̃G�t�F�N�g
	IEffectEmitter* m_effect;

};


#endif // !NORMAL_MAGIC_DEFINED

