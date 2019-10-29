#include "AIMoveCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\CommandParameter.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\EffectID.h>
#include <Game\Effect\PlayerTrailEffectEmitter.h>
#include <Game\Fuzzy/AIMoveFuzzy.h>
#include <Game\Field\FieldData.h>
#include <Game\Element\Element.h>


/// <summary>
/// コンストラクタ
/// </summary>
AIMoveCommand::AIMoveCommand()
	: m_state(MoveState::CollectElement)
	, m_moveInfo()
	, m_euler()
	, m_effectTransform(nullptr)
	, m_pEffect() {
}

/// <summary>
/// AI移動コマンドを初期化する
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::Initialize(Player& player) {
	// プレイヤーの軌跡エフェクトを生成する
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	m_effectTransform.SetParent(&GetTransform(player));
	m_effectTransform.SetPosition(parameter.appearPosOffset);
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerTrail, m_effectTransform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
	effect->SetParent(&m_effectTransform);
	m_pEffect = dynamic_cast<PlayerTrailEffectEmitter*>(effect);
	if (!m_pEffect) {
		ErrorMessage(L"プレイヤーの軌跡エフェクトの生成に失敗しました");
	}
}

/// <summary>
/// AI移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AIMoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed    = parameter.moveSpeed;
	const float move_speed_xy = parameter.moveSpeedXY;
	const float rot_z_limit   = parameter.rotZLimit;
	const float rot_x_limit   = parameter.rotXLimit;
	const float rot_y_limit   = parameter.rotYLimit;
	const float lerp_speed    = parameter.lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	Player::MoveDirection& ref_direction = GetMoveDirection(player);
	const DirectX::SimpleMath::Vector3& other_pos = GetTransform(*GetOtherPlayers(player)[0]).GetLocalPosition();

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();
	DirectX::SimpleMath::Vector3 move(0, 0, 0);

	// AIの処理を行う
	ExecuteAI(player);
	
	if (ref_direction == Player::MoveDirection::Forward && pos.z > 79.0f) {
		ref_direction = Player::MoveDirection::Backward;
	}
	else if (ref_direction == Player::MoveDirection::Backward && pos.z < -79.0f) {
		ref_direction = Player::MoveDirection::Forward;
	}

	// 移動
	const float& nearDistance = parameter.nearDistance;
	bool is_forward = ref_direction == Player::MoveDirection::Forward;
	DirectX::SimpleMath::Vector3 distance = other_pos - pos;
	// すれ違い後の場合
	if (distance.z < 0 == is_forward) {
		// 中心に近づく
		if (fabsf(pos.x) > 0.3f) {
			// 進みたい方向の判定
			if (pos.x > 0 == is_forward) {
				m_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, -rot_y_limit, lerp_speed);
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI - rot_y_limit, lerp_speed);
				}
			}
			else {
				m_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, rot_y_limit, lerp_speed);
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI + rot_y_limit, lerp_speed);
				}
			}
			move.x = (pos.x > 0 ? -1.0f : 1.0f);
		}
		else {
			m_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed);
			if (is_forward) {
				m_euler.y = Math::Lerp(m_euler.y, 0.0f, lerp_speed);
			}
			else {
				m_euler.y = Math::Lerp(m_euler.y, Math::PI, lerp_speed);
			}
		}

		if (fabsf(pos.y) > 0.5f) {
			if (pos.y > 0) {
				m_euler.x = Math::Lerp(m_euler.x, rot_x_limit, lerp_speed);
				move.y = -1.0f;
			}
			else {
				m_euler.x = Math::Lerp(m_euler.x, -rot_x_limit, lerp_speed);
				move.y = 1.0f;
			}
		}
		else {
			m_euler.x = Math::Lerp(m_euler.x, 0.0f, lerp_speed);
		}
	}
	// すれ違い前の場合
	else {
		if (fabsf(distance.x) < nearDistance) {
			if (distance.x < 0 == is_forward) {
				m_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, rot_y_limit, lerp_speed);
					move.x = 1.0f;
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI + rot_y_limit, lerp_speed);
					move.x = -1.0f;
				}
			}
			else {
				m_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, -rot_y_limit, lerp_speed);
					move.x = -1.0f;
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI - rot_y_limit, lerp_speed);
					move.x = 1.0f;
				}
			}
		}
		else {
			m_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed);
			if (is_forward) {
				m_euler.y = Math::Lerp(m_euler.y, 0.0f, lerp_speed);
			}
			else {
				m_euler.y = Math::Lerp(m_euler.y, Math::PI, lerp_speed);
			}
		}

		if (fabsf(distance.y) < nearDistance) {
			if (distance.y > 0) {
				m_euler.x = Math::Lerp(m_euler.x, rot_x_limit, lerp_speed);
				move.y = -1.0f;
			}
			else {
				m_euler.x = Math::Lerp(m_euler.x, -rot_x_limit, lerp_speed);
				move.y = 1.0f;
			}
		}
		else {
			m_euler.x = Math::Lerp(m_euler.x, 0.0f, lerp_speed);
		}
	}

	move.Normalize();
	move *= move_speed_xy;
	if (is_forward) {
		move.z = 1.0f;
	}
	else {
		move.z = -1.0f;
	}
	pos += move * move_speed*elapsedTime;

	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z));
	GetWorld(player) = ref_transform.GetMatrix();
}

/// <summary>
/// AIの処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::ExecuteAI(Player& player) {
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	// 最も近い敵プレイヤーとの距離
	float distance = 0;
	// 最も近い敵プレイヤーを取得する
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), &distance);
	const PlayerStatus& other_status = GetStatus(*other_player);

	// 最も近い敵プレイヤーとのHPの差
	float hp_gap = status.hp - other_status.hp;
	// SP
	float sp = status.sp;
	// 最も近い敵プレイヤーの方を向いているかどうか
	bool looking_other = IsLookingOther(transform, GetTransform(*other_player).GetPosition());
	// 所持しているエレメントの数
	int has_element_num = GetHaveElements(player).size();
	// 最も近いエレメントとの距離
	float element_distance = GetNearestElementDistance(pos);

	// ファジーによるAIの計算
	AIMoveFuzzy fuzzy;
	fuzzy.SetInputData(AIMoveInputCrisp(hp_gap, sp, distance, looking_other, has_element_num, element_distance));
	fuzzy.Execute();
	AIMoveOutputCrisp output_crisp = fuzzy.GetOutputData();

	// ステートを変更して処理を行う
	switch (output_crisp.state) {
	case AIMoveOutputCrisp::State::CollectElement:
		m_state = MoveState::CollectElement;
		ExecuteCollectElement(player);
		break;
	case AIMoveOutputCrisp::State::Chase:
		m_state = MoveState::Chase;
		ExecuteChase(player);
		break;
	case AIMoveOutputCrisp::State::Evade:
		m_state = MoveState::Evade;
		ExecuteEvade(player);
		break;
	default:
		//ErrorMessage(L"AI移動コマンドのクリスプ出力の値が不正です");
		break;
	}
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
const Player* AIMoveCommand::GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* retDistance) {
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

/// <summary>
/// 敵プレイヤーの方を向いているか調べる
/// </summary>
/// <param name="transform">自プレイヤーの姿勢</param>
/// <param name="otherPos">敵プレイヤーの座標</param>
/// <returns></returns>
bool AIMoveCommand::IsLookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos) {
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 dir2 = otherPos - transform.GetPosition();
	
	return dir.Dot(dir2) > 0;
}

/// <summary>
/// 最も近いエレメントとの距離を取得する
/// </summary>
/// <param name="pos">自プレイヤーの座標</param>
/// <returns>
/// 最も近いエレメントとの距離
/// </returns>
float AIMoveCommand::GetNearestElementDistance(const DirectX::SimpleMath::Vector3& pos) {
	float min_distance_square = 10000000.0f;
	const FieldData* field_data = ServiceLocater<FieldData>::Get();
	for (std::vector<Element*>::const_iterator itr = LamdaUtils::FindIf(*field_data->pElements, LamdaUtils::NotNull());
		itr != field_data->pElements->cend();
		LamdaUtils::FindIfNext(itr,field_data->pElements->cend(), LamdaUtils::NotNull())) {
		float distance_square = DirectX::SimpleMath::Vector3::DistanceSquared(pos, (*itr)->GetPos());
		// 他のエレメントより近いなら、距離を更新してポインタを記憶する
		if (distance_square < min_distance_square) {
			min_distance_square = distance_square;
		}
	}

	return std::sqrtf(min_distance_square);
}

/// <summary>
/// エレメント収集の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::ExecuteCollectElement(Player& player) {
	player;
}

/// <summary>
/// 敵プレイヤーの追跡の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::ExecuteChase(Player& player) {
	player;
}

/// <summary>
/// 敵プレイヤーからの逃避の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::ExecuteEvade(Player& player) {
	player;
}

