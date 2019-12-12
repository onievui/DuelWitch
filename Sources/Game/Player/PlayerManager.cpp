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
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, Math::PI2 / static_cast<int>(PlayerID::Num));

	// �v���C���[�𐶐�����
	for (int i = 0; i < static_cast<int>(PlayerID::Num); ++i) {
		// �����Â�]������
		player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
		m_players.emplace_back(std::make_unique<Player>(static_cast<PlayerID>(i), player_pos));
	}
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
	// �������Ă��Ȃ��v���C���[���폜����
	m_players.erase(std::remove_if(m_players.begin(), m_players.end(), std::mem_fn(&Player::IsDead)), m_players.end());

	// �v���C���[���X�V����
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->SetOtherPlayers(m_players);
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

/// <summary>
/// �v���C���[1���������Ă��邩���肷��
/// </summary>
/// <returns>
/// true : �������Ă���
/// false : �������Ă��Ȃ�
/// </returns>
bool PlayerManager::Player1Win() {
	// �������Ă���v���C���[�����Ȃ��Ȃ�i�����ɓ|�ꂽ�ꍇ�j���������ɂ���
	if (m_players.size() == 0) {
		return true;
	}
	// �v���C���[1�������������Ă���Ȃ珟��
	return std::all_of(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return player->GetPlayerID() == PlayerID::Player1; });
}

/// <summary>
/// �v���C���[1���s�k���Ă��邩���肷��
/// </summary>
/// <returns>
/// true : �s�k���Ă���
/// false : �s�k���Ă��Ȃ�
/// </returns>
bool PlayerManager::Player1Lose() {
	// �������Ă���v���C���[�����Ȃ��Ȃ�i�����ɓ|�ꂽ�ꍇ�j���������ɂ���
	if (m_players.size() == 0) {
		return false;
	}

	// �v���C���[1���������Ă��Ȃ��Ȃ�s�k
	return std::all_of(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return player->GetPlayerID() != PlayerID::Player1; });
}
