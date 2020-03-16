#include "UserRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include <Game\Camera\TargetCamera.h>


/// <summary>
/// ���[�U�`��R�}���h������������
/// </summary>
/// <param name="player">�v���C���[</param>
void UserRenderCommand::Initialize(Player& player) {
	m_targetIconInfo.resize(GetOtherPlayers(player).size());

	// ��HP�̏�����
	m_redHp = m_preHp = GetStatus(player).hp;
	m_redHpTime = 0.0f;
}

/// <summary>
/// ���[�U�`��R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void UserRenderCommand::Execute(Player& player, const DX::StepTimer& timer) {
	timer;
	// �^�[�Q�b�g�̕����������A�C�R�����X�V����
	UpdateIcon(player);
	
	// �Ə����X�V����
	UpdateAiming(player, timer);

	// ��HP���X�V����
	UpdateRedHpBar(player, timer);
}

/// <summary>
/// �v���C���[��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {

	// �v���C���[�̃��f����`�悷��
	RenderPlayerModel(player, view, proj);

	// ����v���C���[�̃A�C�R����\������
	RenderEnemeyIcon(player, spriteBatch);

	// �����G�������g��`�悷��
	RenderElements(player, spriteBatch);

	// HP�o�[��`�悷��
	RenderHpBar(player, spriteBatch);
	
	// SP�o�[��`�悷��
	RenderSpBar(player, spriteBatch);
	
	// �Ə���`�悷��
	RenderAiming(player, spriteBatch);
	
}

/// <summary>
/// �^�[�Q�b�g�̕����������A�C�R�����X�V����
/// </summary>
/// <param name="player">�v���C���[</param>
void UserRenderCommand::UpdateIcon(Player& player) {
	// ����v���C���[����ʊO�ɂ���ꍇ�A������\�����邽�߂�UI�̍��W���v�Z����
	const Camera& camera = GetCamera(player);
	const TargetCamera* target_camera = dynamic_cast<const TargetCamera*>(&camera);

	// �A�C�R���̕\���t���O�����Z�b�g����
	for (std::vector<UserRenderCommand::TargetIconInfo>::iterator itr = m_targetIconInfo.begin(); itr != m_targetIconInfo.end(); ++itr) {
		itr->enable = false;
	}

	// ��p���擾���āA��ʊO���ǂ����͈̔͂����߂�
	float area_angle;
	if (target_camera) {
		area_angle = target_camera->GetFov();
	}
	else {
		// �J�������Ȃ��ꍇ�A��p45���Ɖ��肷��
		area_angle = Math::QuarterPI;
	}
	const DirectX::SimpleMath::Vector3& camera_pos = camera.GetEyePosition();
	DirectX::SimpleMath::Vector3 camera_dir = camera.GetEyeVector();
	const std::vector<Player*>& other_players = GetOtherPlayers(player);

	for (unsigned int i = 0; i < other_players.size(); i++) {
		const DirectX::SimpleMath::Vector3& other_player_pos = GetTransform(*other_players[i]).GetLocalPosition();
		DirectX::SimpleMath::Vector3 other_dir = other_player_pos - camera_pos;
		float angle = std::acosf(camera_dir.Dot(other_dir) / (camera_dir.Length()*other_dir.Length()));
		// �J�����̌����ƓG�̕��������̊p�x�ȓ��ł���Ε`�悵�Ȃ�
		if (angle < area_angle) {
			break;
		}
		// �G�v���C���[�ւ̃x�N�g��
		DirectX::SimpleMath::Vector3 vec;
		// �J�����̕�����-Z�����Ɍ������]�s��𐶐�����
		if (camera_dir.z <= 0.0f) {
			DirectX::SimpleMath::Quaternion rotation = Math::CreateQuaternionFromVector3(camera_dir, -DirectX::SimpleMath::Vector3::UnitZ);
			// �G�v���C���[�ւ̃x�N�g��������̕����̉�]������
			vec = DirectX::SimpleMath::Vector3::Transform(other_dir, rotation);
		}
		else {
			DirectX::SimpleMath::Quaternion rotation = Math::CreateQuaternionFromVector3(camera_dir, DirectX::SimpleMath::Vector3::UnitZ);
			// �G�v���C���[�ւ̃x�N�g��������̕����̉�]������
			vec = DirectX::SimpleMath::Vector3::Transform(other_dir, rotation);
			// Y����180�x��]������
			vec *= DirectX::SimpleMath::Vector3(-1, 1, -1);


		}
		constexpr float screen_offset = 50.0f;
		const DirectX11* directX = ServiceLocater<DirectX11>::Get();
		DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));
		// �A�C�R���̍��W���v�Z����
		m_targetIconInfo[i].pos = CalculateIconPos(vec, screen_size, DirectX::SimpleMath::Vector2(screen_offset, screen_offset));
		// �A�C�R���̕`���L���ɂ���
		m_targetIconInfo[i].enable = true;
		// �A�C�R���̃L����ID����ݒ肷��
		m_targetIconInfo[i].charaId = GetStatus(*other_players[i]).charaId;
	}
}


/// <summary>
/// �^�[�Q�b�g�̕����������A�C�R���̈ʒu���v�Z����
/// </summary>
/// <param name="vec">�^�[�Q�b�g�̕���</param>
/// <param name="screenSize">��ʃT�C�Y</param>
/// <param name="screenOffset">�A�C�R���̕`��ʒu�̐���</param>
/// <returns>
/// �^�[�Q�b�g�̕����������A�C�R���̈ʒu
/// </returns>
DirectX::SimpleMath::Vector2 UserRenderCommand::CalculateIconPos(const DirectX::SimpleMath::Vector3& vec,
	const DirectX::SimpleMath::Vector2& screenSize, const DirectX::SimpleMath::Vector2& screenOffset) {
	// �^�[�Q�b�g�̕����������A�C�R���̈ʒu
	DirectX::SimpleMath::Vector2 icon_pos;

	// ���s�̏ꍇ
	if (vec.y == 0.0f) {
		icon_pos.y = screenSize.y * 0.5f;
		// ���E�̊m�F
		icon_pos.x = (vec.x <= 0.0f ? screenOffset.x : screenSize.x - screenOffset.x);
		return icon_pos;
	}
	// �����̏ꍇ
	if (vec.x == 0.0f) {
		icon_pos.x = screenSize.x * 0.5f;
		// �㉺�̊m�F
		icon_pos.y = (vec.y >= 0.0f ? screenOffset.y : screenSize.y - screenOffset.y);
		return icon_pos;
	}
	
	// �΂߂̏ꍇ
	// �I�t�Z�b�g���������X�N���[���̔����̃T�C�Y
	DirectX::SimpleMath::Vector2 harf_screen_size = screenSize * 0.5f - screenOffset;
	// �X�N���[���̏c����
	float screen_aspect = harf_screen_size.x / harf_screen_size.y;
	// �^�[�Q�b�g�̕����̏c����
	float target_aspect = std::fabsf(vec.x / vec.y);
	
	// ���E�����̏ꍇ
	if (target_aspect >= screen_aspect) {
		// ���E�̊m�F
		icon_pos.x = (vec.x <= 0.0f ? screenOffset.x : screenSize.x - screenOffset.x);
		// Y���W�̌v�Z
		icon_pos.y = screenSize.y*0.5f - (harf_screen_size.x / std::fabsf(vec.x))*vec.y;
	}
	// �㉺�����̏ꍇ
	else {
		// �㉺�̊m�F
		icon_pos.y = (vec.y >= 0.0f ? screenOffset.y : screenSize.y - screenOffset.y);
		// X���W�̌v�Z
		icon_pos.x = screenSize.x*0.5f + (harf_screen_size.y / std::fabsf(vec.y))*vec.x;
	}
	return icon_pos;
}

/// <summary>
/// �Ə����X�V����
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void UserRenderCommand::UpdateAiming(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const PlayerStatus& status = GetStatus(player);
	m_aimingPos = ServiceLocater<MouseWrapper>::Get()->GetPos();
	const float aiming_rotation_speed = Math::HarfPI;
	// �u�[�X�g���Ȃ�Ə��̉�]�𑬂�����
	if (status.isBoosting) {
		m_aimigRotation += elapsed_time * aiming_rotation_speed*2.0f;
	}
	else {
		m_aimigRotation += elapsed_time * aiming_rotation_speed;
	}
}

/// <summary>
/// ����v���C���[�̃A�C�R����`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderEnemeyIcon(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	player;
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaIcon);
	for (unsigned int i = 0; i < m_targetIconInfo.size(); ++i) {
		// �L���ȃA�C�R���̂ݕ`�悷��
		if (m_targetIconInfo[i].enable) {
			spriteBatch->Draw(texture->GetResource(m_targetIconInfo[i].charaId).Get(),
				m_targetIconInfo[i].pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), 0,
				texture->GetCenter(), DirectX::SimpleMath::Vector2(0.4f, 0.4f));
		}
	}
}

/// <summary>
/// �����G�������g��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderElements(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const std::list<ElementID>& have_elements = GetHaveElements(player);
	const PlayerStatus& status = GetStatus(player);
	int i = have_elements.size() - 1;
	// �ŏ��Ɏ擾�����G�������g�����ɗ���悤�ɕ\������
	for (std::list<ElementID>::const_reverse_iterator itr = have_elements.rbegin(); itr != have_elements.rend(); ++itr) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);
		spriteBatch->Draw(texture->GetResource(static_cast<int>(*itr)).Get(), DirectX::SimpleMath::Vector2(90 + i * 50.0f, 540.0f), nullptr,
			DirectX::Colors::White, 0, texture->GetCenter(), DirectX::SimpleMath::Vector2::One*(i <= status.chargeLevel ? 1.5f : 1.1f));
		--i;
	}
}

/// <summary>
/// HP�o�[��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::HpBar);
	DirectX::SimpleMath::Vector2 pos(30.0f, 650.0f);
	DirectX::SimpleMath::Vector2 scale(0.55f, 0.5f);

	// ���̕�����`�悷��
	spriteBatch->Draw(texture->GetResource(2).Get(), pos, nullptr,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// �摜�̋�`���쐬����
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const PlayerStatus& status = GetStatus(player);
	RECT rect{ 0,0,0,0 };
	// ��HP�̊����ɉ����ĕ`��͈͂����߂�
	rect.right = static_cast<LONG>(size.x*m_redHp / status.maxHp);
	rect.bottom = static_cast<LONG>(size.y);

	// �Ԃ̕�����`�悷��
	spriteBatch->Draw(texture->GetResource(1).Get(), pos, &rect,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// HP�̊����ɉ����ĕ`��͈͂����߂�
	rect.right = static_cast<LONG>(size.x*status.hp / status.maxHp);

	// �΂̕�����`�悷��
	spriteBatch->Draw(texture->GetResource(0).Get(), pos, &rect,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

}

/// <summary>
/// SP�o�[��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderSpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::SpBar);
	DirectX::SimpleMath::Vector2 pos(30.0f, 610.0f);
	DirectX::SimpleMath::Vector2 scale(0.45f, 0.3f);

	// ���̕�����`�悷��
	spriteBatch->Draw(texture->GetResource(1).Get(), pos, nullptr,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// �摜�̋�`���쐬����
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const PlayerStatus& status = GetStatus(player);
	RECT rect;
	rect.left = 0; rect.top = 0;
	// SP�̊����ɉ����ĕ`��͈͂����߂�
	rect.right = static_cast<LONG>(size.x*status.sp / status.maxSp);
	rect.bottom = static_cast<LONG>(size.y);

	// SP��������������͈Â�����
	DirectX::SimpleMath::Color color = DirectX::Colors::White;
	if (status.spDecreaseTimer > 0.0f) {
		color = DirectX::SimpleMath::Color(0.7f, 0.7f, 0.7f, 1);
	}
	spriteBatch->Draw(texture->GetResource(0).Get(), pos, &rect,
		color, 0, DirectX::SimpleMath::Vector2::Zero, scale);
}

/// <summary>
/// �Ə���`�悷��
/// </summary>
/// /// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderAiming(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	// ���b�N�I�����Ȃ�Ə��̐F��ς���
	bool lock_on = (GetStatus(player).lockOnPlayerID != -1);

	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicAiming);
	spriteBatch->Draw(texture->GetResource(lock_on ? 1 : 0).Get(),
		m_aimingPos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), m_aimigRotation,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.25f, 0.25f));
}

