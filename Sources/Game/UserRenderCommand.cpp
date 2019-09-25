#include "UserRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include "TargetCamera.h"


/// <summary>
/// ���[�U�`��R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void UserRenderCommand::Execute(Player& player, const DX::StepTimer& timer) {
	timer;
	// ����v���C���[����ʊO�ɂ���ꍇ�A������\�����邽�߂�UI�̍��W���v�Z����
	const Camera& camera = GetCamera(player);
	const TargetCamera* target_camera = dynamic_cast<const TargetCamera*>(&camera);
	// ��p���擾���āA��ʊO���ǂ����͈̔͂����߂�
	float area_angle;
	if (target_camera) {
		area_angle = target_camera->GetFov();
	}
	else {
		area_angle = Math::HarfPI;
	}
	const DirectX::SimpleMath::Vector3& camera_pos = camera.GetEyePosition();
	const DirectX::SimpleMath::Vector3& enemy_pos = GetTransform(GetOtherPlayer(player)).GetPosition();
	DirectX::SimpleMath::Vector3 camera_dir = camera.GetCameraVector();
	DirectX::SimpleMath::Vector3 other_dir = enemy_pos - camera_pos;
	float angle = std::acosf(camera_dir.Dot(other_dir) / (camera_dir.Length()*other_dir.Length()));
	// �J�����̌����ƓG�̕��������̊p�x�ȓ��ł���Ώ������Ȃ�
	if (angle < area_angle) {
		// �A�C�R���̕`��𖳌��ɂ���
		m_enableRenderTargetIcon = false;
		return;
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
	m_targetIconPos = CalculateIconPos(vec, screen_size, DirectX::SimpleMath::Vector2(screen_offset,screen_offset));
	// �A�C�R���̕`���L���ɂ���
	m_enableRenderTargetIcon = true;

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
	// �_���[�W��̓v���C���[��_�ł�����
	const auto& ref_status = GetStatus(player);
	if (ref_status.damageTimer <= 0.0f || sin(ref_status.damageTimer*Math::PI2 * 2) > 0) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::BloomModel)->GetResource();
		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		const DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();
		// ���f����`�悷��
		model->Draw(context, *states, GetTransform(player).GetMatrix(), view, proj);
		// �����蔻���`�悷��
		GetCollider(player).Render(view, proj);
	}

	// ����v���C���[����ʊO�ɂ���ꍇ�A�A�C�R����\������
	if (m_enableRenderTargetIcon) {
		RenderEnemeyIcon(player, spriteBatch);
	}

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
/// ����v���C���[�̃A�C�R����`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderEnemeyIcon(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	player;
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaIcon);
	spriteBatch->Draw(texture->GetResource().Get(),
		m_targetIconPos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.2f, 0.2f));
}

/// <summary>
/// �����G�������g��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderElements(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const std::list<ElementID>& have_elements = GetHaveElements(player);
	int i = have_elements.size() - 1;
	for (std::list<ElementID>::const_reverse_iterator itr = have_elements.rbegin(); itr != have_elements.rend(); ++itr) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);
		spriteBatch->Draw(texture->GetResource(static_cast<int>(*itr)).Get(), DirectX::SimpleMath::Vector2(20 + i * 40.0f, 550.0f), nullptr,
			DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One*(i == 0 ? 1.5f : 1.25f));
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
	DirectX::SimpleMath::Vector2 pos(20.0f, 690.0f);
	DirectX::SimpleMath::Vector2 scale(0.5f, 0.5f);

	spriteBatch->Draw(texture->GetResource(2).Get(), pos, nullptr,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// �摜�̋�`���쐬����
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const auto& status = GetStatus(player);
	RECT rect;
	rect.left = 0; rect.top = 0;
	// HP�̊����ɉ����ĕ`��͈͂����߂�
	rect.right = static_cast<LONG>(size.x*status.hp / status.maxHp);
	rect.bottom = static_cast<LONG>(size.y);



	spriteBatch->Draw(texture->GetResource(1).Get(), pos, &rect,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);
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
	DirectX::SimpleMath::Vector2 pos(20.0f, 650.0f);
	DirectX::SimpleMath::Vector2 scale(0.4f, 0.3f);

	spriteBatch->Draw(texture->GetResource(1).Get(), pos, nullptr,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// �摜�̋�`���쐬����
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const auto& status = GetStatus(player);
	RECT rect;
	rect.left = 0; rect.top = 0;
	// SP�̊����ɉ����ĕ`��͈͂����߂�
	rect.right = static_cast<LONG>(size.x*status.sp / status.maxSp);
	rect.bottom = static_cast<LONG>(size.y);

	spriteBatch->Draw(texture->GetResource(0).Get(), pos, &rect,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);
}

/// <summary>
/// �Ə���`�悷��
/// </summary>
/// /// <param name="player">�v���C���[</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::RenderAiming(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	player;
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicAiming);
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();
	spriteBatch->Draw(texture->GetResource().Get(),
		mouse_pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.25f, 0.25f));
}