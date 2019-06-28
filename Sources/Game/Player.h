#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


class MagicManager;


/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public IObject {
public:
	Player(MagicManager* magicManager);
	~Player();

	// �v���C���[���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �v���C���[���J������
	void Lost() override;
	// �v���C���[�𐶐�����
	void Create(const std::wstring& fileName, const std::wstring& directory = L"");
	// �v���C���[��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �v���C���[�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// �v���C���[�̓����蔻����擾����
	const SphereCollider* GetCollider() const override;

private:
	// �ړ����s��
	void Move(const DX::StepTimer& timer);
	// ���@��������
	void CastMagic(const DX::StepTimer& timer);

private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ���f��
	std::unique_ptr<DirectX::Model>        m_model;
	// �p��
	Transform                              m_transform;
	// �������蔻��
	SphereCollider                         m_sphereCollider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix            m_world;
	// ���@�}�l�[�W��
	MagicManager*                          m_pMagicManager;
};


#endif // !PLAYER_DEFINED


