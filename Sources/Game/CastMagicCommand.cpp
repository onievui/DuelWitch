#include "CastMagicCommand.h"
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>
#include "MagicID.h"


/// <summary>
///	�R���X�g���N�^
/// </summary>
CastMagicCommand::CastMagicCommand() 
	: m_state(ChargeState::Idle)
	, m_chargingTime() 
	, m_chargeAllowedLevel() {

}

/// <summary>
/// ���@�r���R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�^�C�}�[</param>
void CastMagicCommand::Execute(Player& player, const DX::StepTimer&  timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	elapsedTime;

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
	DirectX::Mouse::ButtonStateTracker* mouse_tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();

	// �N���b�N���Ė��@�𔭎˂���
	if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
		// �`���[�W�V���b�g�\�Ȃ�`���[�W��Ԃ֑J�ڂ���
		m_chargeAllowedLevel = ChargeAllowedLevel(GetHaveElements(player));
		if (m_chargeAllowedLevel > 0) {
			m_state = ChargeState::Charging;
			return;
		}

		// ���C�̍쐬
		DirectX::SimpleMath::Ray ray = GetCamera(player).ScreenPointToRay(DirectX::SimpleMath::Vector3(mouse_pos.x, mouse_pos.y, 0));
		// ���ʂ̍쐬
		DirectX::SimpleMath::Plane plane = CreatePlaneForMagic(ref_transform);
		float distance;
		if (ray.Intersects(plane, distance)) {
			DirectX::SimpleMath::Vector3 ray_pos = ray.position + ray.direction * distance;
			const DirectX::SimpleMath::Vector3& player_pos = ref_transform.GetLocalPosition();
			DirectX::SimpleMath::Vector3 direction = ray_pos - player_pos;
			direction.Normalize();
			std::list<ElementID>& ref_have_elements = GetHaveElements(player);
			// �G�������g���Ȃ��Ȃ�ʏ햂�@�𔭎˂���
			if (ref_have_elements.empty()) {
				// SP������Ă��邩�m�F����
				Player::Status& status = GetStatus(player);
				if (status.sp >= status.normalMagicSpCost) {
					status.sp -= status.normalMagicSpCost;
					GetMagicManager(player).CreateMagic(MagicInfo(MagicID::Normal, player.GetPlayerID(), 0, 1.0f), player_pos, direction);
				}
			}
			// �G�������g������Ĕ��˂���
			else {
				ElementID element_id = ref_have_elements.front();
				ref_have_elements.pop_front();
				GetMagicManager(player).CreateMagic(element_id, MagicInfo(MagicID::Num,
					player.GetPlayerID(), 0, player.GetMagicPowerRate(element_id)), player_pos, direction);
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
	Player::Status& ref_status = GetStatus(player);
	DirectX::Mouse::ButtonStateTracker* mouse_tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();

	// �N���b�N�������邱�ƂŃ`���[�W����
	if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::HELD) {
		m_chargingTime += static_cast<float>(timer.GetElapsedSeconds());
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
	}
	// �N���b�N�𗣂��Ė��@�𔭎˂���
	else if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::UP) {
		// ���C�̍쐬
		DirectX::SimpleMath::Ray ray = GetCamera(player).ScreenPointToRay(DirectX::SimpleMath::Vector3(mouse_pos.x, mouse_pos.y, 0));
		// ���ʂ̍쐬
		DirectX::SimpleMath::Plane plane = CreatePlaneForMagic(ref_transform);
		float distance;
		if (ray.Intersects(plane, distance)) {
			DirectX::SimpleMath::Vector3 ray_pos = ray.position + ray.direction * distance;
			const DirectX::SimpleMath::Vector3& player_pos = ref_transform.GetLocalPosition();
			DirectX::SimpleMath::Vector3 direction = ray_pos - player_pos;
			direction.Normalize();
			std::list<ElementID>& ref_have_elements = GetHaveElements(player);
			ElementID element_id = ref_have_elements.front();
			// �`���[�W�����������G�������g�������
			for (int i = 0; i < ref_status.chargeLevel + 1; ++i) {
				ref_have_elements.pop_front();
			}
			// ���@�𐶐�����
			GetMagicManager(player).CreateMagic(element_id, MagicInfo(MagicID::Num,
				player.GetPlayerID(), ref_status.chargeLevel, player.GetMagicPowerRate(element_id)), player_pos, direction);
			ref_status.chargeLevel = 0;
			m_chargingTime = 0.0f;
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
