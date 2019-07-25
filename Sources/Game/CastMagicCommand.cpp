#include "CastMagicCommand.h"
#include "MagicFactory.h"

/// <summary>
///	コンストラクタ
/// </summary>
CastMagicCommand::CastMagicCommand() {
	m_mouseTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

/// <summary>
/// 魔法詠唱コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void CastMagicCommand::Execute(Player& player, const DX::StepTimer&  timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());
	elapsedTime;

	auto& ref_transform = GetTransform(player);
	auto mouse_state = DirectX::Mouse::Get().GetState();
	m_mouseTracker->Update(mouse_state);

	// タッチパッドだと左クリックが正常に反応しない
	if (m_mouseTracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
		// レイの作成
		auto ray = GetCamera(player).ScreenPointToRay(DirectX::SimpleMath::Vector3((float)mouse_state.x, (float)mouse_state.y, 0));
		// 平面の作成
		auto plane = CreatePlaneForMagic(ref_transform, GetMoveDirection(player));
		float distance;
		if (ray.Intersects(plane, distance)) {
			auto ray_pos = ray.position + ray.direction * distance;
			auto& player_pos = ref_transform.GetPosition();
			auto direction = ray_pos - player_pos;
			direction.Normalize();
			auto& ref_have_elements = GetHaveElements(player);
			// エレメントがないなら通常魔法を発射する
			if (ref_have_elements.empty()) {
				GetMagicManager(player).CreateMagic(MagicID::Normal , player.GetPlayerID(), player_pos, direction);
			}
			else {
				auto element_id = GetHaveElements(player).front();
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
	const auto& rot = transform.GetRotation();
	auto quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	auto plane_normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ,
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion));
	auto plane_pos = transform.GetPosition() + DirectX::SimpleMath::Vector3::UnitZ *
		(direction == Player::MoveDirection::Forward ? 20.0f : -20.0f);
	auto plane = DirectX::SimpleMath::Plane(plane_pos, DirectX::SimpleMath::Vector3::UnitZ);
	return plane;
}
