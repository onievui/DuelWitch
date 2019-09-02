#include "CastMagicCommand.h"
#include <Utils\ServiceLocater.h>
#include "MagicID.h"


/// <summary>
///	コンストラクタ
/// </summary>
CastMagicCommand::CastMagicCommand() {

}

/// <summary>
/// 魔法詠唱コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void CastMagicCommand::Execute(Player& player, const DX::StepTimer&  timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	elapsedTime;

	Transform& ref_transform = GetTransform(player);
	DirectX::Mouse::ButtonStateTracker* mouse_tracker = ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Get();
	DirectX::Mouse::State mouse_state = mouse_tracker->GetLastState();

	// タッチパッドだと左クリックが正常に反応しない
	if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
		// レイの作成
		DirectX::SimpleMath::Ray ray = GetCamera(player).ScreenPointToRay(
			DirectX::SimpleMath::Vector3(static_cast<float>(mouse_state.x), static_cast<float>(mouse_state.y), 0));
		// 平面の作成
		DirectX::SimpleMath::Plane plane = CreatePlaneForMagic(ref_transform, GetMoveDirection(player));
		float distance;
		if (ray.Intersects(plane, distance)) {
			DirectX::SimpleMath::Vector3 ray_pos = ray.position + ray.direction * distance;
			const DirectX::SimpleMath::Vector3& player_pos = ref_transform.GetPosition();
			DirectX::SimpleMath::Vector3 direction = ray_pos - player_pos;
			direction.Normalize();
			std::list<ElementID>& ref_have_elements = GetHaveElements(player);
			// エレメントがないなら通常魔法を発射する
			if (ref_have_elements.empty()) {
				GetMagicManager(player).CreateMagic(MagicID::Normal , player.GetPlayerID(), player_pos, direction);
			}
			else {
				ElementID element_id = GetHaveElements(player).front();
				GetHaveElements(player).pop_front();
				GetMagicManager(player).CreateMagic(element_id, player.GetPlayerID(), player_pos, direction);
			}
		}
	}
}

/// <summary>
/// 魔法のためのレイ用平面の作成
/// </summary>
/// <param name="transform">姿勢</param>
/// <param name="direction">進行方向</param>
/// <returns>
/// 平面
/// </returns>
DirectX::SimpleMath::Plane CastMagicCommand::CreatePlaneForMagic(const Transform& transform, Player::MoveDirection direction) {
	//DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 plane_pos = transform.GetPosition() + DirectX::SimpleMath::Vector3::UnitZ *
		(direction == Player::MoveDirection::Forward ? 20.0f : -20.0f);
	DirectX::SimpleMath::Plane plane = DirectX::SimpleMath::Plane(plane_pos, DirectX::SimpleMath::Vector3::UnitZ);
	return plane;
}
