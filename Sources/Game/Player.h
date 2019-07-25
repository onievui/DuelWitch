#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include <list>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


class MagicManager;
class IMagic;
class Camera;
class Command;
enum class ElementID;


/// <summary>
/// �v���C���[ID
/// </summary>
enum class PlayerID {
	Player1,
	Player2
};

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public IObject {
	friend class Command;

public:
	// �i�s����
	enum class MoveDirection {
		Forward,
		Backward
	};

public:
	Player(MagicManager* magicManager, PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction);
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
	// �v���C���[ID���擾����
	PlayerID GetPlayerID() const;
	// �G�v���C���[��ݒ肷��
	void SetOtherPlayer(Player* otherPlayer);
	// �J������ݒ肷��
	void SetCamera(Camera* camera);
	// �G�������g�̎擾�������s��
	void GetElement(ElementID elementId);
	// �v���C���[���m�̏Փˏ������s��
	void HitPlayer(const Player& player);
	// ���@�Ƃ̏Փˏ������s��
	void HitMagic(const IMagic* magic);

private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ���f��
	std::unique_ptr<DirectX::Model>        m_model;
	// �v���C���[ID
	PlayerID                               m_id;
	// �i�s����
	MoveDirection                          m_direction;
	// �����G�������g
	std::list<ElementID>                   m_haveElements;
	// �p��
	Transform                              m_transform;
	// �������蔻��
	SphereCollider                         m_sphereCollider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix            m_world;
	// �ړ��R�}���h
	std::unique_ptr<Command>               m_moveCommand;
	// �r���R�}���h
	std::unique_ptr<Command>               m_castCommand;

	// �_���[�W���o�p�^�C�}�[
	float                                  m_damageTimer;

	// �G�v���C���[
	Player*                                m_otherPlayer;
	// ���@�}�l�[�W��
	MagicManager*                          m_pMagicManager;
	// �J����
	Camera*                                m_pCamera;
};


#endif // !PLAYER_DEFINED


