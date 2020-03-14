#include "PlayerManager.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\Resource.h>
#include "Player.h"
#include "PlayerID.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerManager::PlayerManager() {
	// �v���C���[�f�[�^���T�[�r�X���P�[�^�ɓo�^����
	ServiceLocater<PlayerData>::Register(&m_playerData);
	// �v���C���[�f�[�^�̗e�ʂ�ݒ肷��
	m_playerData.transforms.resize(static_cast<int>(PlayerID::Num));

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
	m_players.clear();
	ServiceLocater<PlayerData>::Unregister();
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

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Player1EffectBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBuffer.GetAddressOf());

	// �V�F�[�_�p�萔�̏�����
	m_player1EffectBuffer.time = DirectX::SimpleMath::Vector4(0, 0, 0, 0);
}

/// <summary>
/// �v���C���[�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void PlayerManager::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// �������Ă��Ȃ��v���C���[���폜����
	m_players.erase(std::remove_if(m_players.begin(), m_players.end(), std::mem_fn(&Player::IsDead)), m_players.end());

	// �v���C���[���X�V����
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->SetOtherPlayers(m_players);
		(*itr)->Update(timer);
	}

	// �V�F�[�_�p�萔���X�V����
	m_player1EffectBuffer.time.x += elapsed_time;
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
/// �v���C���[1�̉�ʃG�t�F�N�g��`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void PlayerManager::RenderPlayer1Effect(DirectX::SpriteBatch* spriteBatch) {
	// �v���C���[1���擾����
	std::vector<std::unique_ptr<Player>>::const_iterator player1_itr = std::find_if(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return (player->GetPlayerID() == PlayerID::Player1); });

	// �擾�ł��Ȃ��ꍇ�͏����𔲂���
	if (player1_itr == m_players.cend()) {
		return;
	}

	// �u�[�X�g��ԂȂ�G�t�F�N�g��`�悷��
	if (!(*player1_itr)->IsBoosting()) {
		return;
	}

	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &m_player1EffectBuffer, 0, 0);

	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied(),
		nullptr, nullptr, nullptr, [&]() {
		const PixelShaderResource* shader = ServiceLocater<ResourceManager<PixelShaderResource>>::Get()->GetResource(PixelShaderID::Boost);
		context->PSSetShader(shader->GetResource().Get(), nullptr, 0);
		context->PSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());
	});


	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::BoostEffect);
	spriteBatch->Draw(texture->GetResource().Get(), DirectX::SimpleMath::Vector2(640, 360), nullptr, DirectX::Colors::SkyBlue,
		0.0f, texture->GetCenter(), DirectX::SimpleMath::Vector2(1280 / 256.0f, 720 / 256.0f));
	spriteBatch->End();
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
