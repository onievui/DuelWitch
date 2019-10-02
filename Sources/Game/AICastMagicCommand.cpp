#include "AICastMagicCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\CommandParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"


/// <summary>
/// AI���@�r���R�}���h�N���X
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�^�C�}�[</param>
void AICastMagicCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::ai_cast_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->aiCastparam;

	// �A�˂𐧌�����
	if (m_waitTime > 0) {
		m_waitTime -= elapsedTime;
		return;
	}

	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Quaternion& rot = transform.GetLocalRotation();
	const DirectX::SimpleMath::Vector3& pos = transform.GetLocalPosition();
	const DirectX::SimpleMath::Vector3& target_pos = GetTransform(GetOtherPlayer(player)).GetLocalPosition();

	// �U���͈͂𐧌�����
	const float shotable_angle = parameter.shotableAngle;
	DirectX::SimpleMath::Vector3 direction = target_pos - pos;
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rot);
	float angle = std::acosf(forward.Dot(direction) / (forward.Length()*direction.Length()));
	if (angle > shotable_angle) {
		return;
	}

	direction.Normalize();
	std::list<ElementID>& ref_have_elements = GetHaveElements(player);
	// �G�������g���Ȃ��Ȃ�ʏ햂�@�𔭎˂���
	if (ref_have_elements.empty()) {
		// SP������Ă��邩�m�F����
		Player::Status& status = GetStatus(player);
		if (status.sp >= status.normalMagicSpCost) {
			status.sp -= status.normalMagicSpCost;
			GetMagicManager(player).CreateMagic(MagicID::Normal, 0, player.GetPlayerID(), pos, direction);
		}
	}
	else {
		ElementID element_id = GetHaveElements(player).front();
		GetHaveElements(player).pop_front();
		GetMagicManager(player).CreateMagic(element_id, 0, player.GetPlayerID(), pos, direction);
	}

	const float cast_delay = parameter.castDelay;
	m_waitTime = cast_delay;
}

