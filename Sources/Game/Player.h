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
class RenderCommand;
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
	void Create();
	// �v���C���[��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const;

public:
	// �v���C���[�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// �v���C���[�̓����蔻����擾����
	const Collider* GetCollider() const override;
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
	// �X�e�[�^�X������������
	void InitializeStatus();
	// �X�e�[�^�X���X�V����
	void UpdateStatus(const DX::StepTimer& timer);

public:
	/// <summary>
	/// �v���C���[�X�e�[�^�X
	/// </summary>
	struct Status {
		// �ő�HP
		float                             maxHp;
		// HP
		float                             hp;
		// �O�t���[����HP
		float                             preHp;
		// �ő�SP
		float                             maxSp;
		// SP
		float                             sp;
		// �O�t���[����SP
		float                             preSp;
		// SP�񕜑��x
		float                             spRecoverySpeed;
		// �ʏ햂�@�̏���SP
		float                             normalMagicSpCost;
		// �u�[�X�g���x�{��
		float                             boostSpeedRate;
		// �u�[�X�g����SP
		float                             boostSpCost;

		// �_���[�W�㖳�G���ԃ^�C�}�[
		float                             damageTimer;
		// SP�����^�C�}�[
		float                             spDecreaseTimer;
		// �u�[�X�g�����ǂ���
		bool                              isBoosting;
	};

private:
	// �v���C���[ID
	PlayerID                               m_id;
	// �X�e�[�^�X
	Status                                 m_status;
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
	// �`��R�}���h
	std::unique_ptr<RenderCommand>         m_renderCommand;

	// �G�v���C���[
	Player*                                m_otherPlayer;
	// ���@�}�l�[�W��
	MagicManager*                          m_pMagicManager;
	// �J����
	Camera*                                m_pCamera;
};


#endif // !PLAYER_DEFINED


