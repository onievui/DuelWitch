#include "Player.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CharaStatusParameter.h>
#include "PlayParameterLoader.h"
#include "Command.h"
#include "MoveCommand.h"
#include "AIMoveCommand.h"
#include "CastMagicCommand.h"
#include "AICastMagicCommand.h"
#include "UserRenderCommand.h"
#include "AIRenderCommand.h"
#include "MagicID.h"
#include "IMagic.h"
#include "MagicManager.h"
#include "Camera.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="id">�v���C���[ID</param>
/// <param name="pos">�������W</param>
/// <param name="direction">�i�s����</param>
Player::Player(PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction)
	: m_id(id)
	, m_status()
	, m_direction(direction)
	, m_haveElements()
	, m_transform(pos, DirectX::SimpleMath::Vector3(0, (m_direction == MoveDirection::Forward ? 0 : Math::PI), 0))
	, m_sphereCollider(&m_transform, 0.75f, DirectX::SimpleMath::Vector3(0,0.5f,0)) 
	, m_pMagicManager()
	, m_pCamera() {

	// �v���C���[�ƓG�v���C���[�ŃR�}���h��ς���
	if (id == PlayerID::Player1) {
		m_moveCommand = std::make_unique<MoveCommand>();
		m_castCommand = std::make_unique<CastMagicCommand>();
		m_renderCommand = std::make_unique<UserRenderCommand>();
	}
	else {
		m_moveCommand = std::make_unique<AIMoveCommand>();
		m_castCommand = std::make_unique<AICastMagicCommand>();
		m_renderCommand = std::make_unique<AIRenderCommand>();
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player() {
}

/// <summary>
/// �v���C���[������������
/// </summary>
/// <param name="pMagicManager">���@�}�l�[�W���ւ̃|�C���^</param>
/// <param name="pCamera">�J�����ւ̃|�C���^</param>
/// <param name="pOtherPlayer">�G�v���C���[�ւ̃|�C���^</param>
void Player::Initialize(MagicManager* pMagicManager, Camera* pCamera, Player* pOtherPlayer) {
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

	// �|�C���^���󂯎��
	m_pMagicManager = pMagicManager;
	m_pCamera = pCamera;
	m_pOtherPlayer = pOtherPlayer;

	// �X�e�[�^�X������������
	InitializeStatus();

	// �R�}���h������������
	m_moveCommand->Initialize(*this);
	m_castCommand->Initialize(*this);
	m_renderCommand->Initialize(*this);

	//ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::PlayerTrail, m_transform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ)
	//	->SetParent(&m_transform);
	//if (m_id == PlayerID::Player1) {
	//	static Transform tra;
	//	tra.SetPosition(DirectX::SimpleMath::Vector3(0, 3, 40));
	//	ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::FireMagic, m_transform.GetPosition(), DirectX::SimpleMath::Vector3::UnitX)
	//		->SetParent(&tra);
	//}

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
	// �`��̂��߂̏������s��
	m_renderCommand->Execute(*this, timer);

	// �X�e�[�^�X���X�V����
	UpdateStatus(timer);
	
}

/// <summary>
/// �v���C���[���������
/// </summary>
void Player::Lost() {

}

/// <summary>
/// �v���C���[��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	proj, view;
}

/// <summary>
/// �v���C���[��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
	m_renderCommand->Render(*this, view, proj, spriteBatch);
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

	DirectX::SimpleMath::Vector3 my_pos = m_transform.GetLocalPosition();
	const DirectX::SimpleMath::Vector3& other_pos = player.m_transform.GetLocalPosition();
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
	// ���G���ԂłȂ���΃_���[�W���󂯂�
	if (m_status.damageTimer <= 0.0f) {
		m_status.hp -= 10.0f;
		m_status.damageTimer = 3.0f;
	}
}

/// <summary>
/// �t�B�[���h�Ƃ̏Փ˔�����s��
/// </summary>
/// <param name="hitPos">�Փ˓_</param>
void Player::HitField(const DirectX::SimpleMath::Vector3& hitPos) {
	DirectX::SimpleMath::Vector3 change = hitPos - m_sphereCollider.GetPos();
	m_transform.SetPosition(m_transform.GetLocalPosition() + change);
}

/// <summary>
/// �X�e�[�^�X������������
/// </summary>
void Player::InitializeStatus() {
	const CharaStatusParameter::chara1_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCharaStatusParameter()->chara1Param;

	m_status.maxHp = m_status.hp = m_status.preHp = parameter.maxHp;
	m_status.maxSp = m_status.sp = m_status.preSp = parameter.maxSp;
	m_status.spRecoverySpeed = parameter.spRecoverySpeed;
	m_status.normalMagicSpCost = parameter.normalMagicSpCost;
	m_status.boostSpeedRate = parameter.boostSpeedRate;
	m_status.boostSpCost = parameter.boostSpCost;

	m_status.damageTimer = 0.0f;
	m_status.spDecreaseTimer = 0.0f;
	m_status.isBoosting = false;
}

/// <summary>
/// �X�e�[�^�X���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Player::UpdateStatus(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_status.preHp = m_status.hp;

	// SP���񕜂���
	if (m_status.preSp > m_status.sp) {
		// SP�����������͉񕜂��Ȃ�
		const float sp_decrease_time = 2.0f;
		m_status.spDecreaseTimer = sp_decrease_time;
	}
	else {
		m_status.spDecreaseTimer -= elapsed_time;
	}
	if (m_status.spDecreaseTimer <= 0) {
		m_status.sp = std::min(m_status.sp + m_status.spRecoverySpeed*elapsed_time, m_status.maxSp);
	}
	m_status.preSp = m_status.sp;
	

	// �_���[�W�㖳�G���Ԃ����炷
	m_status.damageTimer -= elapsed_time;
}

