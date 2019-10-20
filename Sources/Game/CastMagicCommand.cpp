#include "CastMagicCommand.h"
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>
#include "MagicID.h"


/// <summary>
///	コンストラクタ
/// </summary>
CastMagicCommand::CastMagicCommand() 
	: m_state(ChargeState::Idle)
	, m_chargingTime() 
	, m_chargeAllowedLevel() {

}

/// <summary>
/// 魔法詠唱コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void CastMagicCommand::Execute(Player& player, const DX::StepTimer&  timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	elapsedTime;

	// 状態に応じた処理を行う
	switch (m_state) {
	case CastMagicCommand::ChargeState::Idle:
		ExecuteIdle(player, timer);
		break;
	case CastMagicCommand::ChargeState::Charging:
		ExecuteCharging(player, timer);
		break;
	default:
		ErrorMessage(L"魔法詠唱コマンドの状態が不正です");
		break;
	}

	
}

/// <summary>
/// 通常状態の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void CastMagicCommand::ExecuteIdle(Player& player, const DX::StepTimer& timer) {
	timer;

	Transform& ref_transform = GetTransform(player);
	DirectX::Mouse::ButtonStateTracker* mouse_tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();

	// クリックして魔法を発射する
	if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
		// チャージショット可能ならチャージ状態へ遷移する
		m_chargeAllowedLevel = ChargeAllowedLevel(GetHaveElements(player));
		if (m_chargeAllowedLevel > 0) {
			m_state = ChargeState::Charging;
			return;
		}

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
					GetMagicManager(player).CreateMagic(MagicInfo(MagicID::Normal, player.GetPlayerID(), 0, 1.0f), player_pos, direction);
				}
			}
			// エレメントを消費して発射する
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
/// チャージ中の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void CastMagicCommand::ExecuteCharging(Player& player, const DX::StepTimer& timer) {
	Transform& ref_transform = GetTransform(player);
	Player::Status& ref_status = GetStatus(player);
	DirectX::Mouse::ButtonStateTracker* mouse_tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();

	// クリックし続けることでチャージする
	if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::HELD) {
		m_chargingTime += static_cast<float>(timer.GetElapsedSeconds());
		// 一定時間チャージすることでチャージ段階が大きくなる
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
	// クリックを離して魔法を発射する
	else if (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::UP) {
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
			ElementID element_id = ref_have_elements.front();
			// チャージした分だけエレメントを消費する
			for (int i = 0; i < ref_status.chargeLevel + 1; ++i) {
				ref_have_elements.pop_front();
			}
			// 魔法を生成する
			GetMagicManager(player).CreateMagic(element_id, MagicInfo(MagicID::Num,
				player.GetPlayerID(), ref_status.chargeLevel, player.GetMagicPowerRate(element_id)), player_pos, direction);
			ref_status.chargeLevel = 0;
			m_chargingTime = 0.0f;
			m_state = ChargeState::Idle;
		}
	}
}

/// <summary>
/// チャージショットが出来る段階を調べる
/// </summary>
/// <param name="elements">エレメント</param>
/// <returns>
/// チャージショットが出来る段階（0は不可）
/// </returns>
int CastMagicCommand::ChargeAllowedLevel(const std::list<ElementID>& elements) {
	// エレメントが1つ以下なら不可
	if (elements.size() < 2) {
		return 0;
	}
	// 同一エレメントが二つ並んでいなければ不可
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
