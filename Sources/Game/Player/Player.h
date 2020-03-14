#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include <list>
#include <Game\Object\IObject.h>
#include <Game\Object\Transform.h>
#include <Game\Collision\SphereCollider.h>
#include "PlayerStatus.h"


class MagicManager;
class IMagic;
class Camera;
class Command;
class RenderCommand;
enum class ElementID;
enum class PlayerID;


/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public IObject {
	// �v���C���[�̃v���C�x�[�g�ȃ����o�ɃA�N�Z�X�ł���悤�ɂ���
	friend class Command;

public:
	// �G�������g�������
	static constexpr int HAVE_ELEMENT_MAX = 10;

public:
	// �R���X�g���N�^
	Player(PlayerID id, const DirectX::SimpleMath::Vector3& pos);
	// �f�X�g���N�^
	~Player();

public:
	// �v���C���[������������
	void Initialize(MagicManager* pMagicManager, Camera* pCamera, std::vector<std::unique_ptr<Player>>& pOtherPlayers);
	// �v���C���[���X�V����
	void Update(const DX::StepTimer& timer) override;
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
	// ���̃v���C���[��ݒ肷��
	void SetOtherPlayers(std::vector<std::unique_ptr<Player>>& pOtherPlayers);
	// �v���C���[��HP��0�ȉ����擾����
	bool IsDead() const;
	// �v���C���[���u�[�X�g�ړ������擾����
	bool IsBoosting() const;
	// ���@�̃_���[�W�{�����擾����
	float GetMagicPowerRate(ElementID elementId) const;

public:
	// �G�������g�̎擾�������s��
	void GetElement(ElementID elementId);
	// �v���C���[���m�̏Փˏ������s��
	void HitPlayer(const Player& player);
	// ���@�Ƃ̏Փˏ������s��
	void HitMagic(const IMagic* magic);
	// �t�B�[���h�Ƃ̏Փˏ������s��
	void HitField(const DirectX::SimpleMath::Vector3& hitPos);

private:
	// �X�e�[�^�X������������
	void InitializeStatus();
	// �X�e�[�^�X���X�V����
	void UpdateStatus(const DX::StepTimer& timer);

private:
	// �v���C���[ID
	PlayerID                               m_id;
	// �X�e�[�^�X
	PlayerStatus                           m_status;
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
	std::vector<Player*>                   m_pOtherPlayers;
	// ���@�}�l�[�W��
	MagicManager*                          m_pMagicManager;
	// �J����
	Camera*                                m_pCamera;
};


#endif // !PLAYER_DEFINED


