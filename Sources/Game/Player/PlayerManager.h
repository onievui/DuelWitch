#pragma once


#include <Framework\StepTimer.h>


class Player;
class MagicManager;
class Camera;


/// <summary>
/// �v���C���[�}�l�[�W���N���X
/// </summary>
class PlayerManager {
public:
	// �R���X�g���N�^
	PlayerManager();
	// �f�X�g���N�^
	~PlayerManager();

public:
	// �v���C���[�}�l�[�W��������������
	void Initialize(MagicManager* pMagicManager, Camera* camera);
	// �v���C���[�}�l�[�W�����X�V����
	void Update(const DX::StepTimer& timer);
	// �v���C���[��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const;

public:
	// �v���C���[���擾����
	std::vector<std::unique_ptr<Player>>* GetPlayers();

private:
	std::vector<std::unique_ptr<Player>> m_players;

};