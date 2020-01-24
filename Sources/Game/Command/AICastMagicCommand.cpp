#include "AICastMagicCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\CommandParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Magic\MagicID.h>


/// <summary>
/// AI魔法詠唱コマンドを初期化する
/// </summary>
/// <param name="player"></param>
void AICastMagicCommand::Initialize(Player& player) {
	player;
	// 0にすると出現した直後に発射してしまうため調整
	m_waitTime = 5.0f;
}

/// <summary>
/// AI魔法詠唱コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AICastMagicCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::ai_cast_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->aiCastparam;

	// 連射を制限する
	if (m_waitTime > 0) {
		m_waitTime -= elapsedTime;
		return;
	}

	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Quaternion& rot = transform.GetLocalRotation();
	const DirectX::SimpleMath::Vector3& pos = transform.GetLocalPosition();
	// 他のプレイヤーがいない場合は処理しない
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), nullptr);
	if (!other_player) {
		return;
	}
	const DirectX::SimpleMath::Vector3& target_pos = GetTransform(*other_player).GetLocalPosition();

	// 攻撃範囲を制限する
	const float shotable_angle = parameter.shotableAngle;
	DirectX::SimpleMath::Vector3 direction = target_pos - pos;
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rot);
	if (Math::BetweenAngle(forward, direction) > shotable_angle) {
		return;
	}

	direction.Normalize();
	std::list<ElementID>& ref_have_elements = GetHaveElements(player);
	// エレメントがないなら通常魔法を発射する
	if (ref_have_elements.empty()) {
		// SPに余裕があるか判定する
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
/// 最も近い敵プレイヤーを取得する
/// </summary>
/// <param name="pos">自プレイヤーの座標</param>
/// <param name="otherPlayers">敵プレイヤーの配列</param>
/// <param name="retDistance">最も近い敵プレイヤーとの距離を格納するポインタ</param>
/// <returns>
/// 最も近い敵プレイヤーへのポインタ
/// </returns>
const Player* AICastMagicCommand::GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* retDistance) {
	const Player* nearest_player = nullptr;
	float min_distance_square = 10000000.0f;
	for (std::vector<Player*>::const_iterator itr = otherPlayers.cbegin(); itr != otherPlayers.cend(); ++itr) {
		float distance_square = DirectX::SimpleMath::Vector3::DistanceSquared(pos, GetTransform(**itr).GetPosition());
		// 他のプレイヤーより近いなら、距離を更新してポインタを記憶する
		if (distance_square < min_distance_square) {
			min_distance_square = distance_square;
			nearest_player = *itr;
		}
	}

	// 距離をポインタに渡す
	if (retDistance) {
		*retDistance = std::sqrtf(min_distance_square);
	}
	return nearest_player;
}

