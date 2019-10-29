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
/// �R���X�g���N�^
/// </summary>
AIMoveCommand::AIMoveCommand()
	: m_state(MoveState::CollectElement)
	, m_moveInfo()
	, m_euler()
	, m_effectTransform(nullptr)
	, m_pEffect() {
}

/// <summary>
/// AI�ړ��R�}���h������������
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::Initialize(Player& player) {
	// �v���C���[�̋O�ՃG�t�F�N�g�𐶐�����
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	m_effectTransform.SetParent(&GetTransform(player));
	m_effectTransform.SetPosition(parameter.appearPosOffset);
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerTrail, m_effectTransform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
	effect->SetParent(&m_effectTransform);
	m_pEffect = dynamic_cast<PlayerTrailEffectEmitter*>(effect);
	if (!m_pEffect) {
		ErrorMessage(L"�v���C���[�̋O�ՃG�t�F�N�g�̐����Ɏ��s���܂���");
	}
}

/// <summary>
/// AI�ړ��R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�^�C�}�[</param>
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

	// AI�̏������s��
	ExecuteAI(player);
	
	if (ref_direction == Player::MoveDirection::Forward && pos.z > 79.0f) {
		ref_direction = Player::MoveDirection::Backward;
	}
	else if (ref_direction == Player::MoveDirection::Backward && pos.z < -79.0f) {
		ref_direction = Player::MoveDirection::Forward;
	}

	// �ړ�
	const float& nearDistance = parameter.nearDistance;
	bool is_forward = ref_direction == Player::MoveDirection::Forward;
	DirectX::SimpleMath::Vector3 distance = other_pos - pos;
	// ����Ⴂ��̏ꍇ
	if (distance.z < 0 == is_forward) {
		// ���S�ɋ߂Â�
		if (fabsf(pos.x) > 0.3f) {
			// �i�݂��������̔���
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
	// ����Ⴂ�O�̏ꍇ
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
/// AI�̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::ExecuteAI(Player& player) {
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	// �ł��߂��G�v���C���[�Ƃ̋���
	float distance = 0;
	// �ł��߂��G�v���C���[���擾����
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), &distance);
	const PlayerStatus& other_status = GetStatus(*other_player);

	// �ł��߂��G�v���C���[�Ƃ�HP�̍�
	float hp_gap = status.hp - other_status.hp;
	// SP
	float sp = status.sp;
	// �ł��߂��G�v���C���[�̕��������Ă��邩�ǂ���
	bool looking_other = IsLookingOther(transform, GetTransform(*other_player).GetPosition());
	// �������Ă���G�������g�̐�
	int has_element_num = GetHaveElements(player).size();
	// �ł��߂��G�������g�Ƃ̋���
	float element_distance = GetNearestElementDistance(pos);

	// �t�@�W�[�ɂ��AI�̌v�Z
	AIMoveFuzzy fuzzy;
	fuzzy.SetInputData(AIMoveInputCrisp(hp_gap, sp, distance, looking_other, has_element_num, element_distance));
	fuzzy.Execute();
	AIMoveOutputCrisp output_crisp = fuzzy.GetOutputData();

	// �X�e�[�g��ύX���ď������s��
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
		//ErrorMessage(L"AI�ړ��R�}���h�̃N���X�v�o�͂̒l���s���ł�");
		break;
	}
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
const Player* AIMoveCommand::GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* retDistance) {
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

/// <summary>
/// �G�v���C���[�̕��������Ă��邩���ׂ�
/// </summary>
/// <param name="transform">���v���C���[�̎p��</param>
/// <param name="otherPos">�G�v���C���[�̍��W</param>
/// <returns></returns>
bool AIMoveCommand::IsLookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos) {
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 dir2 = otherPos - transform.GetPosition();
	
	return dir.Dot(dir2) > 0;
}

/// <summary>
/// �ł��߂��G�������g�Ƃ̋������擾����
/// </summary>
/// <param name="pos">���v���C���[�̍��W</param>
/// <returns>
/// �ł��߂��G�������g�Ƃ̋���
/// </returns>
float AIMoveCommand::GetNearestElementDistance(const DirectX::SimpleMath::Vector3& pos) {
	float min_distance_square = 10000000.0f;
	const FieldData* field_data = ServiceLocater<FieldData>::Get();
	for (std::vector<Element*>::const_iterator itr = LamdaUtils::FindIf(*field_data->pElements, LamdaUtils::NotNull());
		itr != field_data->pElements->cend();
		LamdaUtils::FindIfNext(itr,field_data->pElements->cend(), LamdaUtils::NotNull())) {
		float distance_square = DirectX::SimpleMath::Vector3::DistanceSquared(pos, (*itr)->GetPos());
		// ���̃G�������g���߂��Ȃ�A�������X�V���ă|�C���^���L������
		if (distance_square < min_distance_square) {
			min_distance_square = distance_square;
		}
	}

	return std::sqrtf(min_distance_square);
}

/// <summary>
/// �G�������g���W�̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::ExecuteCollectElement(Player& player) {
	player;
}

/// <summary>
/// �G�v���C���[�̒ǐՂ̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::ExecuteChase(Player& player) {
	player;
}

/// <summary>
/// �G�v���C���[����̓����̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::ExecuteEvade(Player& player) {
	player;
}

