#include "Player.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include "Command.h"
#include "MoveCommand.h"
#include "AIMoveCommand.h"
#include "CastMagicCommand.h"
#include "AICastMagicCommand.h"
#include "MagicID.h"
#include "IMagic.h"
#include "MagicManager.h"
#include "Camera.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager">���@�}�l�[�W��</param>
/// <param name="id">�v���C���[ID</param>
/// <param name="pos">�������W</param>
/// <param name="direction">�i�s����</param>
Player::Player(MagicManager* magicManager, PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction)
	: m_states()
	, m_id(id)
	, m_direction(direction)
	, m_haveElements()
	, m_transform(pos, DirectX::SimpleMath::Vector3(0, (m_direction == MoveDirection::Forward ? 0 : Math::PI), 0))
	, m_sphereCollider(&m_transform, 0.75f, DirectX::SimpleMath::Vector3(0,0.5f,0)) 
	, m_pMagicManager(magicManager)
	, m_pCamera() {
	if (id == PlayerID::Player1) {
		m_moveCommand = std::make_unique<MoveCommand>();
		m_castCommand = std::make_unique<CastMagicCommand>();
	}
	else {
		m_moveCommand = std::make_unique<AIMoveCommand>();
		m_castCommand = std::make_unique<AICastMagicCommand>();
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player() {
}

/// <summary>
/// �v���C���[���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Player::Update(const DX::StepTimer& timer) {
	// �ړ����s��
	m_moveCommand->Execute(*this, timer);

	// ���@�𔭓�����
	m_castCommand->Execute(*this, timer);

	m_damageTimer -= static_cast<float>(timer.GetElapsedSeconds());
}

/// <summary>
/// �v���C���[���������
/// </summary>
void Player::Lost() {
	m_states.reset();
}

/// <summary>
/// �v���C���[�𐶐�����
/// </summary>
void Player::Create() {
	// �f�o�C�X�̎擾
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();

	// �R�����X�e�[�g���쐬����
	m_states = std::make_unique<DirectX::CommonStates>(device);

	// �G�t�F�N�g��ݒ肷��
	const ModelResource* modelResource = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::BloomModel);
	modelResource->GetResource()->UpdateEffects([](DirectX::IEffect* effect) {
		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights) {
			lights->SetLightingEnabled(true);
			lights->SetPerPixelLighting(true);
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
			lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*0.3f);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
	});

}

/// <summary>
/// �v���C���[��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	if (m_damageTimer <= 0.0f || sin(m_damageTimer*Math::PI2*2)>0) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::BloomModel)->GetResource();
		model->Draw(context, *m_states, m_transform.GetMatrix(), view, proj);
		m_sphereCollider.Render(view, proj);
	}
}

void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	if (m_damageTimer <= 0.0f || sin(m_damageTimer*Math::PI2 * 2) > 0) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::BloomModel)->GetResource();
		model->Draw(context, *m_states, m_transform.GetMatrix(), view, proj);
		m_sphereCollider.Render(view, proj);
	}
	if (m_id == PlayerID::Player2)
		return;

	int i = m_haveElements.size() - 1;
	for (std::list<ElementID>::const_reverse_iterator itr = m_haveElements.rbegin(); itr != m_haveElements.rend(); ++itr) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);
		spriteBatch->Draw(texture->GetResource(static_cast<int>(*itr)).Get(), DirectX::SimpleMath::Vector2(20 + i * 40.0f, 630.0f), nullptr,
			DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2(1.5f, 1.5f));
		--i;
	}

	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicAiming);
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();
	spriteBatch->Draw(texture->GetResource().Get(),
		mouse_pos, nullptr, DirectX::SimpleMath::Vector4(1,1,1,0.8f), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.25f, 0.25f));
}

/// <summary>
/// �v���C���[�̍s����擾����
/// </summary>
/// <returns>
/// �v���C���[�̍s��
/// </returns>
const DirectX::SimpleMath::Matrix& Player::GetMatrix() const {
	return m_world;
}

/// <summary>
/// �v���C���[�̓����蔻����擾����
/// </summary>
/// <returns>
/// �����蔻��
/// </returns>
const Collider* Player::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// �v���C���[ID���擾����
/// </summary>
/// <returns>
/// �v���C���[ID
/// </returns>
PlayerID Player::GetPlayerID() const {
	return m_id;
}

/// <summary>
/// �G�v���C���[��ݒ肷��
/// </summary>
/// <param name="otherPlayer">�G�v���C���[�ւ̃|�C���^</param>
void Player::SetOtherPlayer(Player* otherPlayer) {
	m_otherPlayer = otherPlayer;
}

/// <summary>
/// �J������ݒ肷��
/// </summary>
/// <param name="camera">�J�����ւ̃|�C���^</param>
void Player::SetCamera(Camera* camera) {
	m_pCamera = camera;
}

/// <summary>
/// �G�������g�̎擾�������s��
/// </summary>
/// <param name="elementId">�G�������gID</param>
void Player::GetElement(ElementID elementId) {
	m_haveElements.push_back(elementId);
}

/// <summary>
/// �v���C���[���m�̏Փˏ������s��
/// </summary>
/// <param name="player">����v���C���[</param>
void Player::HitPlayer(const Player& player) {
	constexpr float reflect_distance = 0.25f;

	DirectX::SimpleMath::Vector3 my_pos = m_transform.GetPosition();
	const DirectX::SimpleMath::Vector3& other_pos = player.m_transform.GetPosition();
	// ����v���C���[�Ɣ�������ɐi��
	float angle = std::atan2f(my_pos.y - other_pos.y, my_pos.x - other_pos.x);
	my_pos += DirectX::SimpleMath::Vector3(std::cosf(angle), std::sinf(angle), 0.0f)*reflect_distance;
	m_transform.SetPosition(my_pos);
}

/// <summary>
/// ���@�Ƃ̏Փˏ������s��
/// </summary>
/// <param name="magic">���@</param>
void Player::HitMagic(const IMagic* magic) {
	if (magic->GetID() == MagicID::Thunder) {
		return;
	}
	if (m_damageTimer <= 0.0f) {
		m_damageTimer = 3.0f;
	}
}

