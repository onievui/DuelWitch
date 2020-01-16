#include "Player.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CharaStatusParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Scene\ShareData\ShareData.h>
#include <Game\Command\Command.h>
#include <Game\Command\MoveCommand.h>
#include <Game\Command\AIMoveCommand.h>
#include <Game\Command\CastMagicCommand.h>
#include <Game\Command\AICastMagicCommand.h>
#include <Game\Command\UserRenderCommand.h>
#include <Game\Command\AIRenderCommand.h>
#include <Game\Element\ElementFactory.h>
#include <Game\Magic\MagicID.h>
#include <Game\Magic\IMagic.h>
#include <Game\Magic\MagicManager.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Camera\Camera.h>
#include <Game\Field\FieldData.h>
#include "PlayerID.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="id">�v���C���[ID</param>
/// <param name="pos">�������W</param>
/// <param name="direction">�i�s����</param>
Player::Player(PlayerID id, const DirectX::SimpleMath::Vector3& pos)
	: m_id(id)
	, m_status()
	, m_haveElements()
	, m_transform(pos)
	, m_sphereCollider(&m_transform, 0.75f, DirectX::SimpleMath::Vector3(0,0.65f,0)) 
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
/// <param name="pOtherPlayers">���̃v���C���[�ւ̃|�C���^�̔z��</param>
void Player::Initialize(MagicManager* pMagicManager, Camera* pCamera, std::vector<std::unique_ptr<Player>>& pOtherPlayers) {
	// �G�t�F�N�g��ݒ肷��
	// ⴃ��f��
	const ModelResource* modelResource = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::Bloom);
	modelResource->GetResource()->UpdateEffects([](DirectX::IEffect* effect) {
		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights) {
			lights->SetLightingEnabled(true);
			lights->SetPerPixelLighting(true);
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
			lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*0.7f);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
	});
	// �L�����N�^�[���f��
	const ModelResource* modelResource2 = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::Chara);
	const std::vector<std::unique_ptr<DirectX::Model>>& charas = modelResource2->GetAllResources();
	for (std::vector<std::unique_ptr<DirectX::Model>>::const_iterator itr = charas.begin(); itr != charas.end(); ++itr) {
		(*itr)->UpdateEffects([](DirectX::IEffect* effect) {
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights) {
				lights->SetLightingEnabled(true);
				lights->SetPerPixelLighting(true);
				lights->SetLightEnabled(0, true);
				lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
				lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*0.8f);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
		});
	}

	// �|�C���^���󂯎��
	m_pMagicManager = pMagicManager;
	m_pCamera = pCamera;
	SetOtherPlayers(pOtherPlayers);

	// �X�e�[�W�̒��S�֌�����
	DirectX::SimpleMath::Vector3 field_pos = ServiceLocater<FieldData>::Get()->fieldCenter;
	DirectX::SimpleMath::Vector3 field_dir = field_pos - m_transform.GetPosition();
	DirectX::SimpleMath::Quaternion rot = Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::UnitZ, field_dir);
	rot.Inverse(rot);
	m_transform.SetRotation(rot);

	// �X�e�[�^�X������������
	InitializeStatus();

	// �R�}���h������������
	m_moveCommand->Initialize(*this);
	m_castCommand->Initialize(*this);
	m_renderCommand->Initialize(*this);

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
	
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Z)) {
		ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::Hit, DirectX::SimpleMath::Vector3::Zero, -DirectX::SimpleMath::Vector3::UnitZ);
	}

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
/// ���̃v���C���[��ݒ肷��
/// </summary>
/// <param name="pOtherPlayers"></param>
void Player::SetOtherPlayers(std::vector<std::unique_ptr<Player>>& pOtherPlayers) {
	// �f�[�^�����Z�b�g����
	m_pOtherPlayers.clear();
	for (std::vector<std::unique_ptr<Player>>::iterator itr = pOtherPlayers.begin(); itr != pOtherPlayers.end(); itr++) {
		// ���g�̃|�C���^�͔��肵�Ȃ�
		if ((*itr).get() != this) {
			m_pOtherPlayers.push_back((*itr).get());
		}
	}
}

/// <summary>
/// �v���C���[��HP��0�ȉ����擾����
/// </summary>
/// <returns>
/// true : ����ł���
/// false : �������Ă���
/// </returns>
bool Player::IsDead() const {
	return m_status.hp <= 0;
}

/// <summary>
/// ���@�̃_���[�W�{�����擾����
/// </summary>
/// <param name="elementId">�G�������gID</param>
/// <returns>
/// �_���[�W�{��
/// </returns>
float Player::GetMagicPowerRate(ElementID elementId) const {
	switch (elementId) {
	case ElementID::Fire:
		return m_status.fireMagicPowerRate;
	case ElementID::Freeze:
		return m_status.freezeMagicPowerRate;
	case ElementID::Thunder:
		return m_status.thunderMagicPowerRate;
	default:
		break;
	}

	ErrorMessage(L"���@�̃_���[�W�{���̎擾�ŕs����ID���n����܂���");
	return 0.0f;
}

/// <summary>
/// �G�������g�̎擾�������s��
/// </summary>
/// <param name="elementId">�G�������gID</param>
void Player::GetElement(ElementID elementId) {
	m_haveElements.push_back(elementId);
	// �G�������g�擾����炷
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::GetElement);
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
		m_status.hp -= magic->GetPower();
		m_status.damageTimer = 3.0f;

		// �_���[�W���ʉ���炷
		ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Damage);
	}

	// �q�b�g�G�t�F�N�g���o��������
	ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::Hit, m_transform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
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
	const std::vector<int>& chara_id = ServiceLocater<ShareData>::Get()->GetSelectCharaID();
	m_status.charaId = chara_id[static_cast<int>(m_id)];

	const CharaStatusParameter::chara1_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCharaStatusParameter()->chara1Param;

	m_status.maxHp = m_status.hp = m_status.preHp = parameter.maxHp;
	m_status.maxSp = m_status.sp = m_status.preSp = parameter.maxSp;

	m_status.spRecoverySpeed       = parameter.spRecoverySpeed;
	m_status.normalMagicSpCost     = parameter.normalMagicSpCost;
	m_status.boostSpeedRate        = parameter.boostSpeedRate;
	m_status.boostSpCost           = parameter.boostSpCost;
	m_status.rollSpCost            = parameter.rollSpCost;
	m_status.firstChargeTime       = parameter.firstChargeTime;
	m_status.secoundChargeTime     = parameter.secoundChargeTime;
	m_status.fireMagicPowerRate    = parameter.fireMagicPowerRate;
	m_status.freezeMagicPowerRate  = parameter.freezeMagicPowerRate;
	m_status.thunderMagicPowerRate = parameter.thunderMagicPowerRate;

	m_status.damageTimer     = 0.0f;
	m_status.spDecreaseTimer = 0.0f;
	m_status.isBoosting      = false;
	m_status.isCharging      = false;
	m_status.chargeLevel     = 0;
	m_status.canCast         = true;
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
	// �Ō��SP������Ă����莞�Ԍo�ߌ�ɉ񕜂��n�߂�
	if (m_status.spDecreaseTimer <= 0) {
		m_status.sp = std::min(m_status.sp + m_status.spRecoverySpeed*elapsed_time, m_status.maxSp);
	}
	m_status.preSp = m_status.sp;
	

	// �_���[�W�㖳�G���Ԃ����炷
	m_status.damageTimer -= elapsed_time;
}

