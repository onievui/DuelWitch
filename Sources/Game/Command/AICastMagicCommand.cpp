#include "AICastMagicCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\CommandParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Magic\MagicID.h>


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
	const DirectX::SimpleMath::Vector3& target_pos = GetTransform(*GetOtherPlayers(player)[0]).GetLocalPosition();

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
		PlayerStatus& status = GetStatus(player);
		if (status.sp >= status.normalMagicSpCost) {
			status.sp -= status.normalMagicSpCost;
			GetMagicManager(player).CreateMagic(MagicInfo(MagicID::Normal,player.GetPlayerID(),0,1.0f), pos, direction);
		}
	}
	else {
		ElementID element_id = GetHaveElements(player).front();
		GetHaveElements(player).pop_front();
		GetMagicManager(player).CreateMagic(element_id, MagicInfo(MagicID::Normal,
			player.GetPlayerID(), 0, player.GetMagicPowerRate(element_id)), pos, direction);
	}

	const float cast_delay = parameter.castDelay;
	m_waitTime = cast_delay;
}
