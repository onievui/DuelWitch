#include "PlayerManager.h"
#include <Utils\MathUtils.h>
#include "Player.h"
#include "PlayerID.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerManager::PlayerManager() {
	// �v���C���[�̏������W
	DirectX::SimpleMath::Vector3 player_pos(0, 0, -75);
	// �v���C���[�̏������W�����炷���߂̉�]�𐶐�����
	DirectX::SimpleMath::Quaternion player_pos_rot =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, Math::PI2 / 3);

	// �v���C���[�𐶐�����
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player1, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player2, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player3, player_pos));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerManager::~PlayerManager() {

}

/// <summary>
/// �v���C���[�}�l�[�W��������������
/// </summary>
/// <param name="pMagicManager">���@�}�l�[�W���ւ̃|�C���^</param>
/// <param name="camera">�J�����ւ̃|�C���^</param>
void PlayerManager::Initialize(MagicManager* pMagicManager, Camera* camera) {
	// �v���C���[������������
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Initialize(pMagicManager, camera, m_players);
	}
}

/// <summary>
/// �v���C���[�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void PlayerManager::Update(const DX::StepTimer& timer) {
	// �v���C���[���X�V����
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Update(timer);
	}
}

/// <summary>
/// �v���C���[��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void PlayerManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	DirectX::SpriteBatch* spriteBatch) const {
	// �v���C���[��`�悷��
	for (std::vector<std::unique_ptr<Player>>::const_iterator itr = m_players.cbegin(); itr != m_players.cend(); ++itr) {
		(*itr)->Render(view, proj, spriteBatch);
	}
}


/// <summary>
/// �v���C���[���擾����
/// </summary>
/// <returns>
/// �v���C���[
/// </returns>
std::vector<std::unique_ptr<Player>>* PlayerManager::GetPlayers() {
	return &m_players;
}
