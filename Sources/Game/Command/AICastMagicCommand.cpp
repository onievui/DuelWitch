#include "AICastMagicCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\CommandParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Magic\MagicID.h>


/// <summary>
/// AI���@�r���R�}���h������������
/// </summary>
/// <param name="player"></param>
void AICastMagicCommand::Initialize(Player& player) {
	player;
	// 0�ɂ���Əo����������ɔ��˂��Ă��܂����ߒ���
	m_waitTime = 5.0f;
}

/// <summary>
/// AI���@�r���R�}���h����������
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
	// ���̃v���C���[�����Ȃ��ꍇ�͏������Ȃ�
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), nullptr);
	if (!other_player) {
		return;
	}
	const DirectX::SimpleMath::Vector3& target_pos = GetTransform(*other_player).GetLocalPosition();

	// �U���͈͂𐧌�����
	const float shotable_angle = parameter.shotableAngle;
	DirectX::SimpleMath::Vector3 direction = target_pos - pos;
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rot);
	if (Math::BetweenAngle(forward, direction) > shotable_angle) {
		return;
	}

	direction.Normalize();
	std::list<ElementID>& ref_have_elements = GetHaveElements(player);
	// �G�������g���Ȃ��Ȃ�ʏ햂�@�𔭎˂���
	if (ref_have_elements.empty()) {
		// SP�ɗ]�T�����邩���肷��
		PlayerStatus& status = GetStatus(player);
		if (status.sp >= status.maxSp*0.9f) {
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

/// <summary>
/// �ł��߂��G�v���C���[���擾����
/// </summary>
/// <param name="pos">���v���C���[�̍��W</param>
/// <param name="otherPlayers">�G�v���C���[�̔z��</param>
/// <param name="retDistance">�ł��߂��G�v���C���[�Ƃ̋������i�[����|�C���^</param>
/// <returns>
/// �ł��߂��G�v���C���[�ւ̃|�C���^
/// </returns>
const Player* AICastMagicCommand::GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* retDistance) {
	const Player* nearest_player = nullptr;
	float min_distance_square = 10000000.0f;
	for (std::vector<Player*>::const_iterator itr = otherPlayers.cbegin(); itr != otherPlayers.cend(); ++itr) {
		float distance_square = DirectX::SimpleMath::Vector3::DistanceSquared(pos, GetTransform(**itr).GetPosition());
		// ���̃v���C���[���߂��Ȃ�A�������X�V���ă|�C���^���L������
		if (distance_square < min_distance_square) {
			min_distance_square = distance_square;
			nearest_player = *itr;
		}
	}

	// �������|�C���^�ɓn��
	if (retDistance) {
		*retDistance = std::sqrtf(min_distance_square);
	}
	return nearest_player;
}

