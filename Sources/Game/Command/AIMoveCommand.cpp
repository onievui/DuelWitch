#include "AIMoveCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Utils\ServiceLocater.h>
#include <Utils\AudioManager.h>
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
/// デストラクタ
/// </summary>
AIMoveCommand::~AIMoveCommand() {
	// エフェクトを消す
	m_pEffect->SetUsed(false);
}

/// <summary>
/// AI移動コマンドを初期化する
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::Initialize(Player& player) {
	// 向きを初期化する
	DirectX::SimpleMath::Matrix rot_matrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetTransform(player).GetRotation());
	m_euler.y = std::atan2f(-rot_matrix._31, rot_matrix._33);

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
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed    = parameter.moveSpeed;
	const float rot_z_limit   = parameter.rotZLimit;
	const float rot_x_limit   = parameter.rotXLimit;
	const float lerp_speed    = parameter.lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	PlayerStatus& ref_status = GetStatus(player);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();

	// AIの処理を行う
	ExecuteAI(player);
	
	// 回転の変化量
	DirectX::SimpleMath::Vector3 change_euler;
	// 回転量
	float rot_speed = (ref_status.isBoosting ? parameter.boostRotSpeed : parameter.rotSpeed);
	
	// 左右移動
	if (m_moveInfo.xVec < 0.0f) {
		change_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = rot_speed * elapsed_time;
	}
	else if (m_moveInfo.xVec > 0.0f) {
		change_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = -rot_speed * elapsed_time;
	}
	//押していないときは戻す
	else {
		change_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed) - m_euler.z;
	}

	// 上下移動
	if (m_moveInfo.yVec > 0.0f) {
		change_euler.x = -rot_speed * elapsed_time;
	}
	else if (m_moveInfo.yVec < 0.0f) {
		change_euler.x = rot_speed * elapsed_time;
	}

	// 斜め移動の場合
	if (!Math::Equal0(change_euler.x) && !Math::Equal0(change_euler.y)) {
		change_euler /= std::sqrtf(2);
	}

	// 回転を計算する
	m_euler += change_euler;
	m_euler.x = Math::Clamp(m_euler.x, -rot_x_limit, rot_x_limit);
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);

	// 移動方向を計算する
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);

	// ブーストを使用しようとしていて、残りSPが10％以上ならブースト移動
	if (m_moveInfo.useBoost && ref_status.sp / ref_status.maxSp >= 0.1f) {
		pos += move * move_speed*elapsed_time*ref_status.boostSpeedRate;
		// SPを減らす
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
		// 敵の音は鳴らさないようにする
		// 音を小さくして鳴らすこと検討する
		// // ブースト開始直後なら効果音を鳴らす
		//if (!ref_status.isBoosting) {
		//	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Boost);
		//}
		ref_status.isBoosting = true;
	}
	// 通常移動
	else {
		pos += move * move_speed*elapsed_time;
		ref_status.isBoosting = false;
	}

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
	// 他のプレイヤーがいない場合は処理しない
	if (!other_player) {
		return;
	}
	const PlayerStatus& other_status = GetStatus(*other_player);

	// 最も近い敵プレイヤーとのHPの差
	float hp_gap = status.hp - other_status.hp;
	// SP
	float sp = status.sp;
	// 最も近い敵プレイヤーの方を向いているかどうか
	//bool looking_other = IsLookingOther(transform, GetTransform(*other_player).GetPosition());
	// 最も近い敵プレイヤーへの向き具合
	float looking_other = LookingOther(transform, GetTransform(*other_player).GetPosition());
	// 所持しているエレメントの数
	int has_element_num = GetHaveElements(player).size();
	// 最も取りやすいエレメントとの距離
	float element_distance = 10000000.0f;
	const Element* nearest_element = GetTargetElement(pos);
	if (nearest_element) {
		element_distance = DirectX::SimpleMath::Vector3::Distance(pos, nearest_element->GetPos());
	}

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
/// <returns>
/// true : 向いている
/// false : 向いていない
/// </returns>
bool AIMoveCommand::IsLookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos) {
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 dir2 = otherPos - transform.GetPosition();
	
	return dir.Dot(dir2) > 0;
}

/// <summary>
/// 敵プレイヤーへの向き具合を調べる
/// </summary>
/// <param name="transform">自プレイヤーの姿勢</param>
/// <param name="otherPos">敵プレイヤーの座標</param>
/// <returns>
/// 1 : 正面
/// -1 : 逆
/// </returns>
float AIMoveCommand::LookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos) {
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 dir2 = otherPos - transform.GetPosition();

	return dir.Dot(dir2);
}

/// <summary>
/// 最も取りやすいエレメントを取得する
/// </summary>
/// <param name="transform">プレイヤーの姿勢</param>
/// <returns>
/// 最も取りやすいエレメントへのポインタ
/// </returns>
const Element* AIMoveCommand::GetTargetElement(const Transform& transform) {
	float min_distance_square = 10000000.0f;
	const FieldData* field_data = ServiceLocater<FieldData>::Get();
	const Element* nearest_element = nullptr;

	for (IfIterator<const std::vector<Element*>> itr(*field_data->pElements, LamdaUtils::NotNull()); itr != field_data->pElements->end(); ++itr) {
		float distance_square = DirectX::SimpleMath::Vector3::DistanceSquared(transform.GetPosition(), (*itr)->GetPos());
		
		// 近くて前方にないなら飛ばす（取れないまま旋回し続けるため）
		DirectX::SimpleMath::Vector3 player_dir =
			DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
		DirectX::SimpleMath::Vector3 to_element = (*itr)->GetPos() - transform.GetPosition();
		to_element.Normalize();
		const bool dist_near = (distance_square <= 5.0f*5.0f);
		const bool back = (player_dir.Dot(to_element) < Math::HarfPI);
		if (dist_near && back) {
			continue;
		}
		
		// 他のエレメントより近いなら、距離を更新してポインタを記憶する
		if (distance_square < min_distance_square) {
			min_distance_square = distance_square;
			nearest_element = (*itr);
		}
	}

	return nearest_element;
}

/// <summary>
/// ベクトルの左右判定をする
/// </summary>
/// <param name="dir">基準となるベクトル</param>
/// <param name="otherDir">判定するベクトル</param>
/// <returns>
/// - : 左方向
/// 0 : 正面方向
/// + : 右方向
/// </returns>
float AIMoveCommand::CheckVecX(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir) {
	// XZ方向のベクトルを取得する
	dir.y = 0.0f;
	otherDir.y = 0.0f;

	// 角度が小さい場合は無視する
	constexpr float min_angle = Math::PI / 90;
	if (Math::BetweenAngle(dir, otherDir) < min_angle) {
		return 0.0f;
	}

	// 基準のベクトルに合わせて回転する
	DirectX::SimpleMath::Quaternion rot = Math::CreateQuaternionFromVector3(dir, -DirectX::SimpleMath::Vector3::UnitZ);
	DirectX::SimpleMath::Vector3 rotated_dir = DirectX::SimpleMath::Vector3::Transform(otherDir, rot);
	
	// 回転後のベクトルの向きで判定する
	return rotated_dir.x;
}

/// <summary>
/// ベクトルの上下判定をする
/// </summary>
/// <param name="dir">基準となるベクトル</param>
/// <param name="otherDir">判定するベクトル</param>
/// <returns>
/// - : 下方向
/// 0 : 正面方向
/// + : 上方向
/// </returns>
float AIMoveCommand::CheckVecY(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir) {
	// XY方向のベクトルを取得する
	dir.Normalize();
	dir.x = std::sqrtf(1.0f - dir.y*dir.y);
	dir.z = 0.0f;
	otherDir.Normalize();
	otherDir.x = std::sqrtf(1.0f - otherDir.y*otherDir.y);
	otherDir.z = 0.0f;

	// 角度が小さい場合は無視する
	constexpr float min_angle = Math::PI / 90;
	if (Math::BetweenAngle(dir, otherDir) < min_angle) {
		return 0.0f;
	}

	return otherDir.y - dir.y;
}

/// <summary>
/// エレメント収集の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::ExecuteCollectElement(Player& player) {
	const CommandParameter::move_param::ai_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.aiParam;
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	// 最も近いエレメント
	const Element* nearest_element = GetTargetElement(pos);

	// フィールドにエレメントが存在しない場合はエラー
	if (!nearest_element) {
		ErrorMessage(L"敵AIの処理で収集するエレメントが存在しません");
		return;
	}

	// プレイヤーの方向ベクトル
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	// エレメントへのベクトル
	DirectX::SimpleMath::Vector3 element_dir = nearest_element->GetPos() - pos;

	// X方向の回転を調べる
	m_moveInfo.xVec = CheckVecX(dir, element_dir);
	// Y方向の回転を調べる
	m_moveInfo.yVec = CheckVecY(dir, element_dir);
	// SPに余裕があるならブーストを行う
	m_moveInfo.useBoost = (status.sp >= parameter.collectElementBoostSp);

}

/// <summary>
/// 敵プレイヤーの追跡の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::ExecuteChase(Player& player) {
	const CommandParameter::move_param::ai_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.aiParam;
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	// 最も近い敵プレイヤーを取得する
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), nullptr);

	// プレイヤーの方向ベクトル
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	// 敵プレイヤーの座標を取得する
	DirectX::SimpleMath::Vector3 other_pos = GetTransform(*other_player).GetPosition();
	// 敵プレイヤーへのベクトル
	DirectX::SimpleMath::Vector3 other_dir = other_pos - pos;

	// X方向の回転を調べる
	m_moveInfo.xVec = CheckVecX(dir, other_dir);
	// Y方向の回転を調べる
	m_moveInfo.yVec = CheckVecY(dir, other_dir);
	// 距離が離れており、SPに余裕があるならブーストを行う
	m_moveInfo.useBoost = (status.sp >= parameter.chaseBoostSp &&
		DirectX::SimpleMath::Vector3::Distance(pos, other_pos) > parameter.chaseBoostDistance);

}

/// <summary>
/// 敵プレイヤーからの逃避の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void AIMoveCommand::ExecuteEvade(Player& player) {
	const CommandParameter::move_param::ai_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.aiParam;
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	const FieldData* field_data = ServiceLocater<FieldData>::Get();
	// 最も近い敵プレイヤーを取得する
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), nullptr);
	// フィールドの中心座標を取得する
	const DirectX::SimpleMath::Vector3& field_pos = field_data->fieldCenter;

	// プレイヤーの方向ベクトル
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	// 敵プレイヤーの座標を取得する
	DirectX::SimpleMath::Vector3 other_pos = GetTransform(*other_player).GetPosition();
	// 敵プレイヤーとの距離
	float distance = DirectX::SimpleMath::Vector3::Distance(pos, other_pos);
	// フィールドの中心から逃避先へのベクトル
	DirectX::SimpleMath::Vector3 field_target_dir; 
	{
		DirectX::SimpleMath::Vector3 field_dir = pos - field_pos;
		field_dir.Normalize();
		DirectX::SimpleMath::Vector3 field_other_dir = other_pos - field_pos;
		field_other_dir.Normalize();
		field_target_dir = -(field_dir + field_other_dir);
		field_target_dir.Normalize();
	}
	// 逃避先の座標
	DirectX::SimpleMath::Vector3 target_pos = field_pos + field_target_dir * (field_data->fieldRadius - 5.0f);
	// 逃避先へのベクトル
	DirectX::SimpleMath::Vector3 target_dir = target_pos - pos;

	// X方向の回転を調べる
	m_moveInfo.xVec = CheckVecX(dir, target_dir);
	// Y方向の回転を調べる
	m_moveInfo.yVec = CheckVecY(dir, target_dir);

	// 距離がかなり近い場合
	if (distance < parameter.evadeNearBoostDistance) {
		m_moveInfo.useBoost = (status.sp >= parameter.evadeNearBoostSp);
	}
	// 距離が近い場合
	else if (distance < parameter.evadeFarBoostDistance) {
		m_moveInfo.useBoost = (status.sp >= parameter.evadeFarBoostSp);
	}
	// 距離がある場合
	else {
		m_moveInfo.useBoost = false;
	}

}

