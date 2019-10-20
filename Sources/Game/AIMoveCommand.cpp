#include "AIMoveCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\CommandParameter.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "EffectManager.h"
#include "EffectID.h"
#include "PlayerTrailEffectEmitter.h"


/// <summary>
/// コンストラクタ
/// </summary>
AIMoveCommand::AIMoveCommand()
	: m_euler()
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

