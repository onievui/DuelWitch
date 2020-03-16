#include "CastMagicCommand.h"
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Game\Magic\MagicID.h>


/// <summary>
///	�R���X�g���N�^
/// </summary>
CastMagicCommand::CastMagicCommand() 
	: m_state(ChargeState::Idle)
	, m_chargingTime() 
	, m_chargeAllowedLevel()
	, m_chargingSoundTime() {

}

/// <summary>
/// ���@�r���R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�^�C�}�[</param>
void CastMagicCommand::Execute(Player& player, const DX::StepTimer& timer) {
	// �Ə��𑀍삷��
	ControlAim(player, timer);

	// �Ə��ɓG�v���C���[���d�Ȃ��Ă��邩���肷��
	LockOnOtherPlayer(player);

	// ��Ԃɉ������������s��
	switch (m_state) {
	case CastMagicCommand::ChargeState::Idle:
		ExecuteIdle(player, timer);
		break;
	case CastMagicCommand::ChargeState::Charging:
		ExecuteCharging(player, timer);
		break;
	default:
		ErrorMessage(L"���@�r���R�}���h�̏�Ԃ��s���ł�");
		break;
	}

	
}

/// <summary>
/// �ʏ��Ԃ̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CastMagicCommand::ExecuteIdle(Player& player, const DX::StepTimer& timer) {
	timer;

	Transform& ref_transform = GetTransform(player);

	// �{�^���������Ė��@�𔭎˂���
	if (ServiceLocater<InputManager>::Get()->IsPressed(InputID::Shot)) {
		// �`���[�W�V���b�g�\�Ȃ�`���[�W��Ԃ֑J�ڂ���
		m_chargeAllowedLevel = ChargeAllowedLevel(GetHaveElements(player));
		if (m_chargeAllowedLevel > 0) {
			GetStatus(player).isCharging = true;
			m_state = ChargeState::Charging;
			return;
		}

		// ���ʂ̍쐬
		DirectX::SimpleMath::Plane plane = CreatePlaneForMagic(ref_transform);
		float distance;
		if (m_shotRay.Intersects(plane, distance)) {
			DirectX::SimpleMath::Vector3 hit_pos = m_shotRay.position + m_shotRay.direction * distance;
			const DirectX::SimpleMath::Vector3& player_pos = ref_transform.GetLocalPosition();
			DirectX::SimpleMath::Vector3 direction = hit_pos - player_pos;
			direction.Normalize();
			std::list<ElementID>& ref_have_elements = GetHaveElements(player);
			// �G�������g���Ȃ��Ȃ�ʏ햂�@�𔭎˂���
			if (ref_have_elements.empty()) {
				// SP������Ă��邩�m�F����
				PlayerStatus& ref_status = GetStatus(player);
				if (ref_status.sp >= ref_status.normalMagicSpCost) {
					ref_status.sp -= ref_status.normalMagicSpCost;
					GetMagicManager(player).CreateMagic(
						MagicInfo(MagicID::Normal, player.GetPlayerID(), 0, 1.0f, ref_status.lockOnPlayerID),
						player_pos, direction);
				}
			}
			// �G�������g������Ĕ��˂���
			else {
				ElementID element_id = ref_have_elements.front();
				ref_have_elements.pop_front();
				GetMagicManager(player).CreateMagic(
					element_id,
					MagicInfo(MagicID::Num, player.GetPlayerID(), 0, player.GetMagicPowerRate(element_id), GetStatus(player).lockOnPlayerID),
					player_pos, direction);
			}
		}
	}
}

/// <summary>
/// �`���[�W���̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CastMagicCommand::ExecuteCharging(Player& player, const DX::StepTimer& timer) {
	Transform& ref_transform = GetTransform(player);
	PlayerStatus& ref_status = GetStatus(player);

	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// �{�^�������������邱�ƂŃ`���[�W����
	if (ServiceLocater<InputManager>::Get()->IsDown(InputID::Shot)) {
		m_chargingTime += elapsed_time;
		// �r���ŃG�������g�����������ɑΉ��ł���悤�ɂ���
		m_chargeAllowedLevel = ChargeAllowedLevel(GetHaveElements(player));
		// ��莞�ԃ`���[�W���邱�ƂŃ`���[�W�i�K���傫���Ȃ�
		if (ref_status.chargeLevel < m_chargeAllowedLevel) {
			if (ref_status.chargeLevel == 0 && m_chargingTime >= ref_status.firstChargeTime) {
				++ref_status.chargeLevel;
				m_chargingTime = 0.0f;
			}
			else if (ref_status.chargeLevel == 1 && m_chargingTime >= ref_status.secoundChargeTime) {
				++ref_status.chargeLevel;
				m_chargingTime = 0.0f;
			}
		}
		m_chargingSoundTime -= elapsed_time;
		// ��莞�Ԗ��Ɍ��ʉ���炷
		if (m_chargingSoundTime <= 0.0f) {
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Charge);
			m_chargingSoundTime += CHARGING_SOUND_DELAY_TIME;
		}
	}
	// �{�^���𗣂��Ė��@�𔭎˂���
	else if (ServiceLocater<InputManager>::Get()->IsUp(InputID::Shot)) {
		// ���ʂ̍쐬
		DirectX::SimpleMath::Plane plane = CreatePlaneForMagic(ref_transform);
		float distance;
		if (m_shotRay.Intersects(plane, distance)) {
			DirectX::SimpleMath::Vector3 hit_pos = m_shotRay.position + m_shotRay.direction * distance;
			const DirectX::SimpleMath::Vector3& player_pos = ref_transform.GetLocalPosition();
			DirectX::SimpleMath::Vector3 direction = hit_pos - player_pos;
			direction.Normalize();
			std::list<ElementID>& ref_have_elements = GetHaveElements(player);
			ElementID element_id = ref_have_elements.front();
			// �`���[�W�����������G�������g�������
			for (int i = 0; i < ref_status.chargeLevel + 1; ++i) {
				ref_have_elements.pop_front();
			}
			// ���@�𐶐�����
			GetMagicManager(player).CreateMagic(
				element_id,
				MagicInfo(MagicID::Num, player.GetPlayerID(), ref_status.chargeLevel, player.GetMagicPowerRate(element_id), GetStatus(player).lockOnPlayerID),
				player_pos, direction);
			ref_status.isCharging = false;
			ref_status.chargeLevel = 0;
			m_chargingTime = 0.0f;
			m_chargingSoundTime = 0.0f;
			m_state = ChargeState::Idle;
		}
	}
}

/// <summary>
/// �`���[�W�V���b�g���o����i�K�𒲂ׂ�
/// </summary>
/// <param name="elements">�G�������g</param>
/// <returns>
/// �`���[�W�V���b�g���o����i�K�i0�͕s�j
/// </returns>
int CastMagicCommand::ChargeAllowedLevel(const std::list<ElementID>& elements) {
	// �G�������g��1�ȉ��Ȃ�s��
	if (elements.size() < 2) {
		return 0;
	}
	// ����G�������g�������ł��Ȃ���Εs��
	std::list<ElementID>::const_iterator itr = elements.cbegin();
	int level = 0;
	while (static_cast<unsigned int>(level) < elements.size() - 1) {
		if (*itr != *(std::next(itr, 1))) {
			break;
		}
		++level;
		++itr;
	}
	return level;
}

/// <summary>
/// �Ə��𑀍삷��
/// </summary>
/// <param name="player">�v���C���[/param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CastMagicCommand::ControlAim(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	const InputManager* input_manager = ServiceLocater<InputManager>::Get();
	MouseWrapper* mouse_wrapper = ServiceLocater<MouseWrapper>::Get();

	// �p�b�h���ڑ����̓}�E�X���삷��

	// �p�b�h�ڑ����͉E�X�e�B�b�N�Ń}�E�X�J�[�\���𓮂�����悤�ɂ���
	if (input_manager->IsPadConnected()) {
		DirectX::GamePad::ThumbSticks thumb = ServiceLocater<DirectX::GamePad::ButtonStateTracker>::Get()->GetLastState().thumbSticks;
		DirectX::SimpleMath::Vector2 axis(thumb.rightX, -thumb.rightY);
		// ���x��1�ȏ�Ȃ琳�K������
		if (axis.LengthSquared() > 1.0f) {
			axis.Normalize();
		}
		// �}�E�X�̍��W�ɉE�X�e�B�b�N�̈ړ��ʂ𑫂��Ĕ��f����
		DirectX::SimpleMath::Vector2 mouse_pos = mouse_wrapper->GetPos() + axis * CURSOR_SENSITIVITY*elapsedTime;
		mouse_wrapper->SetPos(mouse_pos);
	}

	// �J��������Ə��ւ̃��C�𐶐�����
	const DirectX::SimpleMath::Vector2& mouse_pos = mouse_wrapper->GetPos();
	m_shotRay = GetCamera(player).ScreenPointToRay(DirectX::SimpleMath::Vector3(mouse_pos.x, mouse_pos.y, 0.0f));
}

/// <summary>
/// �Ə��ɓG�v���C���[���d�Ȃ��Ă��邩���肷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>
/// true : �d�Ȃ��Ă���
/// false : �d�Ȃ��Ă��Ȃ�
/// </returns>
bool CastMagicCommand::LockOnOtherPlayer(Player& player) {
	const std::vector<Player*>& other_players = GetOtherPlayers(player);
	PlayerStatus& ref_status = GetStatus(player);
	const Camera& camera = GetCamera(player);
	float min_distance = 1000000.0f;

	for (std::vector<Player*>::const_iterator itr = other_players.begin(); itr != other_players.end(); ++itr) {
		// �G�v���C���[�̓����蔻���ϊ�����
		const SphereCollider* other_collider = dynamic_cast<const SphereCollider*>((*itr)->GetLockOnCollider());
		// �O�̂��ߌ`�`�F�b�N
		if (!other_collider) {
			continue;
		}
		DirectX::BoundingSphere ray_target(other_collider->GetPos(), other_collider->GetRadius());

		// �Փ˔��������
		float distance;
		if (!m_shotRay.Intersects(ray_target, distance)) {
			continue;
		}
		// �����蔻��̒��S����ʂɎʂ��Ă��邩���肷��
		float fov = camera.GetFov();
		DirectX::SimpleMath::Vector3 target_dir = ray_target.Center - camera.GetEyePosition();
		target_dir.Normalize();
		if (camera.GetEyeVector().Dot(target_dir) < std::cosf(fov*0.5f)) {
			continue;
		}

		// �ł��߂��Ȃ�v���C���[ID���L������
		if (distance < min_distance) {
			min_distance = distance;
			ref_status.lockOnPlayerID = static_cast<int>((*itr)->GetPlayerID());
		}
	}

	// �Փ˂��Ȃ������ꍇ�͖����Ȓl������
	if (min_distance >= 990000.0f) {
		ref_status.lockOnPlayerID = -1;
	}

	return false;
}

/// <summary>
/// ���@�̂��߂̃��C�p���ʂ̍쐬
/// </summary>
/// <param name="transform">�p��</param>
/// <returns>
/// ����
/// </returns>
DirectX::SimpleMath::Plane CastMagicCommand::CreatePlaneForMagic(const Transform& transform) {
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 plane_pos = transform.GetLocalPosition() + normal*20.0f;
	DirectX::SimpleMath::Plane plane = DirectX::SimpleMath::Plane(plane_pos, normal);
	return plane;
}
