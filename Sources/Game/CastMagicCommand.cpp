#include "CastMagicCommand.h"
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>
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
	DirectX::Mouse::ButtonStateTracker* mouse_tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();

	// タッチパッドだと左クリックが正常に反応しない
	if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
		// レイの作成
		DirectX::SimpleMath::Ray ray = GetCamera(player).ScreenPointToRay(DirectX::SimpleMath::Vector3(mouse_pos.x, mouse_pos.y, 0));
		// 平面の作成
		DirectX::SimpleMath::Plane plane = CreatePlaneForMagic(ref_transform);
		float distance;
		if (ray.Intersects(plane, distance)) {
			DirectX::SimpleMath::Vector3 ray_pos = ray.position + ray.direction * distance;
			const DirectX::SimpleMath::Vector3& player_pos = ref_transform.GetLocalPosition();
			DirectX::SimpleMath::Vector3 direction = ray_pos - player_pos;
			direction.Normalize();
			std::list<ElementID>& ref_have_elements = GetHaveElements(player);
			// エレメントがないなら通常魔法を発射する
			if (ref_have_elements.empty()) {
				// SPが足りているか確認する
				Player::Status& status = GetStatus(player);
				if (status.sp >= status.normalMagicSpCost) {
					status.sp -= status.normalMagicSpCost;
					GetMagicManager(player).CreateMagic(MagicID::Normal, player.GetPlayerID(), player_pos, direction);
				}
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
/// <returns>
/// 平面
/// </returns>
DirectX::SimpleMath::Plane CastMagicCommand::CreatePlaneForMagic(const Transform& transform) {
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 plane_pos = transform.GetLocalPosition() + normal*20.0f;
	DirectX::SimpleMath::Plane plane = DirectX::SimpleMath::Plane(plane_pos, normal);
	return plane;
}
