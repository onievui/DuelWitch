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
/// �f�X�g���N�^
/// </summary>
AIMoveCommand::~AIMoveCommand() {
	// �G�t�F�N�g������
	m_pEffect->SetUsed(false);
}

/// <summary>
/// AI�ړ��R�}���h������������
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::Initialize(Player& player) {
	// ����������������
	DirectX::SimpleMath::Matrix rot_matrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetTransform(player).GetRotation());
	m_euler.y = std::atan2f(-rot_matrix._31, rot_matrix._33);

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
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed    = parameter.moveSpeed;
	const float rot_z_limit   = parameter.rotZLimit;
	const float rot_x_limit   = parameter.rotXLimit;
	const float lerp_speed    = parameter.lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	PlayerStatus& ref_status = GetStatus(player);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();

	// AI�̏������s��
	ExecuteAI(player);
	
	// ��]�̕ω���
	DirectX::SimpleMath::Vector3 change_euler;
	// ��]��
	float rot_speed = (ref_status.isBoosting ? parameter.boostRotSpeed : parameter.rotSpeed);
	
	// ���E�ړ�
	if (m_moveInfo.xVec < 0.0f) {
		change_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = rot_speed * elapsed_time;
	}
	else if (m_moveInfo.xVec > 0.0f) {
		change_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = -rot_speed * elapsed_time;
	}
	//�����Ă��Ȃ��Ƃ��͖߂�
	else {
		change_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed) - m_euler.z;
	}

	// �㉺�ړ�
	if (m_moveInfo.yVec > 0.0f) {
		change_euler.x = -rot_speed * elapsed_time;
	}
	else if (m_moveInfo.yVec < 0.0f) {
		change_euler.x = rot_speed * elapsed_time;
	}

	// �΂߈ړ��̏ꍇ
	if (!Math::Equal0(change_euler.x) && !Math::Equal0(change_euler.y)) {
		change_euler /= std::sqrtf(2);
	}

	// ��]���v�Z����
	m_euler += change_euler;
	m_euler.x = Math::Clamp(m_euler.x, -rot_x_limit, rot_x_limit);
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);

	// �ړ��������v�Z����
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);

	// �u�[�X�g���g�p���悤�Ƃ��Ă��āA�c��SP��10���ȏ�Ȃ�u�[�X�g�ړ�
	if (m_moveInfo.useBoost && ref_status.sp / ref_status.maxSp >= 0.1f) {
		pos += move * move_speed*elapsed_time*ref_status.boostSpeedRate;
		// SP�����炷
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
		// �G�̉��͖炳�Ȃ��悤�ɂ���
		// �������������Ė炷���ƌ�������
		// // �u�[�X�g�J�n����Ȃ���ʉ���炷
		//if (!ref_status.isBoosting) {
		//	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Boost);
		//}
		ref_status.isBoosting = true;
	}
	// �ʏ�ړ�
	else {
		pos += move * move_speed*elapsed_time;
		ref_status.isBoosting = false;
	}

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
	// ���̃v���C���[�����Ȃ��ꍇ�͏������Ȃ�
	if (!other_player) {
		return;
	}
	const PlayerStatus& other_status = GetStatus(*other_player);

	// �ł��߂��G�v���C���[�Ƃ�HP�̍�
	float hp_gap = status.hp - other_status.hp;
	// SP
	float sp = status.sp;
	// �ł��߂��G�v���C���[�̕��������Ă��邩�ǂ���
	//bool looking_other = IsLookingOther(transform, GetTransform(*other_player).GetPosition());
	// �ł��߂��G�v���C���[�ւ̌����
	float looking_other = LookingOther(transform, GetTransform(*other_player).GetPosition());
	// �������Ă���G�������g�̐�
	int has_element_num = GetHaveElements(player).size();
	// �ł����₷���G�������g�Ƃ̋���
	float element_distance = 10000000.0f;
	const Element* nearest_element = GetTargetElement(pos);
	if (nearest_element) {
		element_distance = DirectX::SimpleMath::Vector3::Distance(pos, nearest_element->GetPos());
	}

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
/// <returns>
/// true : �����Ă���
/// false : �����Ă��Ȃ�
/// </returns>
bool AIMoveCommand::IsLookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos) {
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 dir2 = otherPos - transform.GetPosition();
	
	return dir.Dot(dir2) > 0;
}

/// <summary>
/// �G�v���C���[�ւ̌�����𒲂ׂ�
/// </summary>
/// <param name="transform">���v���C���[�̎p��</param>
/// <param name="otherPos">�G�v���C���[�̍��W</param>
/// <returns>
/// 1 : ����
/// -1 : �t
/// </returns>
float AIMoveCommand::LookingOther(const Transform& transform, const DirectX::SimpleMath::Vector3& otherPos) {
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	DirectX::SimpleMath::Vector3 dir2 = otherPos - transform.GetPosition();

	return dir.Dot(dir2);
}

/// <summary>
/// �ł����₷���G�������g���擾����
/// </summary>
/// <param name="transform">�v���C���[�̎p��</param>
/// <returns>
/// �ł����₷���G�������g�ւ̃|�C���^
/// </returns>
const Element* AIMoveCommand::GetTargetElement(const Transform& transform) {
	float min_distance_square = 10000000.0f;
	const FieldData* field_data = ServiceLocater<FieldData>::Get();
	const Element* nearest_element = nullptr;

	for (IfIterator<const std::vector<Element*>> itr(*field_data->pElements, LamdaUtils::NotNull()); itr != field_data->pElements->end(); ++itr) {
		float distance_square = DirectX::SimpleMath::Vector3::DistanceSquared(transform.GetPosition(), (*itr)->GetPos());
		
		// �߂��đO���ɂȂ��Ȃ��΂��i���Ȃ��܂ܐ��񂵑����邽�߁j
		DirectX::SimpleMath::Vector3 player_dir =
			DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
		DirectX::SimpleMath::Vector3 to_element = (*itr)->GetPos() - transform.GetPosition();
		to_element.Normalize();
		const bool dist_near = (distance_square <= 5.0f*5.0f);
		const bool back = (player_dir.Dot(to_element) < Math::HarfPI);
		if (dist_near && back) {
			continue;
		}
		
		// ���̃G�������g���߂��Ȃ�A�������X�V���ă|�C���^���L������
		if (distance_square < min_distance_square) {
			min_distance_square = distance_square;
			nearest_element = (*itr);
		}
	}

	return nearest_element;
}

/// <summary>
/// �x�N�g���̍��E���������
/// </summary>
/// <param name="dir">��ƂȂ�x�N�g��</param>
/// <param name="otherDir">���肷��x�N�g��</param>
/// <returns>
/// - : ������
/// 0 : ���ʕ���
/// + : �E����
/// </returns>
float AIMoveCommand::CheckVecX(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir) {
	// XZ�����̃x�N�g�����擾����
	dir.y = 0.0f;
	otherDir.y = 0.0f;

	// �p�x���������ꍇ�͖�������
	constexpr float min_angle = Math::PI / 90;
	if (Math::BetweenAngle(dir, otherDir) < min_angle) {
		return 0.0f;
	}

	// ��̃x�N�g���ɍ��킹�ĉ�]����
	DirectX::SimpleMath::Quaternion rot = Math::CreateQuaternionFromVector3(dir, -DirectX::SimpleMath::Vector3::UnitZ);
	DirectX::SimpleMath::Vector3 rotated_dir = DirectX::SimpleMath::Vector3::Transform(otherDir, rot);
	
	// ��]��̃x�N�g���̌����Ŕ��肷��
	return rotated_dir.x;
}

/// <summary>
/// �x�N�g���̏㉺���������
/// </summary>
/// <param name="dir">��ƂȂ�x�N�g��</param>
/// <param name="otherDir">���肷��x�N�g��</param>
/// <returns>
/// - : ������
/// 0 : ���ʕ���
/// + : �����
/// </returns>
float AIMoveCommand::CheckVecY(DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 otherDir) {
	// XY�����̃x�N�g�����擾����
	dir.Normalize();
	dir.x = std::sqrtf(1.0f - dir.y*dir.y);
	dir.z = 0.0f;
	otherDir.Normalize();
	otherDir.x = std::sqrtf(1.0f - otherDir.y*otherDir.y);
	otherDir.z = 0.0f;

	// �p�x���������ꍇ�͖�������
	constexpr float min_angle = Math::PI / 90;
	if (Math::BetweenAngle(dir, otherDir) < min_angle) {
		return 0.0f;
	}

	return otherDir.y - dir.y;
}

/// <summary>
/// �G�������g���W�̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::ExecuteCollectElement(Player& player) {
	const CommandParameter::move_param::ai_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.aiParam;
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	// �ł��߂��G�������g
	const Element* nearest_element = GetTargetElement(pos);

	// �t�B�[���h�ɃG�������g�����݂��Ȃ��ꍇ�̓G���[
	if (!nearest_element) {
		ErrorMessage(L"�GAI�̏����Ŏ��W����G�������g�����݂��܂���");
		return;
	}

	// �v���C���[�̕����x�N�g��
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	// �G�������g�ւ̃x�N�g��
	DirectX::SimpleMath::Vector3 element_dir = nearest_element->GetPos() - pos;

	// X�����̉�]�𒲂ׂ�
	m_moveInfo.xVec = CheckVecX(dir, element_dir);
	// Y�����̉�]�𒲂ׂ�
	m_moveInfo.yVec = CheckVecY(dir, element_dir);
	// SP�ɗ]�T������Ȃ�u�[�X�g���s��
	m_moveInfo.useBoost = (status.sp >= parameter.collectElementBoostSp);

}

/// <summary>
/// �G�v���C���[�̒ǐՂ̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::ExecuteChase(Player& player) {
	const CommandParameter::move_param::ai_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.aiParam;
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	// �ł��߂��G�v���C���[���擾����
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), nullptr);

	// �v���C���[�̕����x�N�g��
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	// �G�v���C���[�̍��W���擾����
	DirectX::SimpleMath::Vector3 other_pos = GetTransform(*other_player).GetPosition();
	// �G�v���C���[�ւ̃x�N�g��
	DirectX::SimpleMath::Vector3 other_dir = other_pos - pos;

	// X�����̉�]�𒲂ׂ�
	m_moveInfo.xVec = CheckVecX(dir, other_dir);
	// Y�����̉�]�𒲂ׂ�
	m_moveInfo.yVec = CheckVecY(dir, other_dir);
	// ����������Ă���ASP�ɗ]�T������Ȃ�u�[�X�g���s��
	m_moveInfo.useBoost = (status.sp >= parameter.chaseBoostSp &&
		DirectX::SimpleMath::Vector3::Distance(pos, other_pos) > parameter.chaseBoostDistance);

}

/// <summary>
/// �G�v���C���[����̓����̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
void AIMoveCommand::ExecuteEvade(Player& player) {
	const CommandParameter::move_param::ai_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.aiParam;
	const PlayerStatus& status = GetStatus(player);
	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	const FieldData* field_data = ServiceLocater<FieldData>::Get();
	// �ł��߂��G�v���C���[���擾����
	const Player* other_player = GetNearestPlayer(pos, GetOtherPlayers(player), nullptr);
	// �t�B�[���h�̒��S���W���擾����
	const DirectX::SimpleMath::Vector3& field_pos = field_data->fieldCenter;

	// �v���C���[�̕����x�N�g��
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, transform.GetRotation());
	// �G�v���C���[�̍��W���擾����
	DirectX::SimpleMath::Vector3 other_pos = GetTransform(*other_player).GetPosition();
	// �G�v���C���[�Ƃ̋���
	float distance = DirectX::SimpleMath::Vector3::Distance(pos, other_pos);
	// �t�B�[���h�̒��S���瓦���ւ̃x�N�g��
	DirectX::SimpleMath::Vector3 field_target_dir; 
	{
		DirectX::SimpleMath::Vector3 field_dir = pos - field_pos;
		field_dir.Normalize();
		DirectX::SimpleMath::Vector3 field_other_dir = other_pos - field_pos;
		field_other_dir.Normalize();
		field_target_dir = -(field_dir + field_other_dir);
		field_target_dir.Normalize();
	}
	// �����̍��W
	DirectX::SimpleMath::Vector3 target_pos = field_pos + field_target_dir * (field_data->fieldRadius - 5.0f);
	// �����ւ̃x�N�g��
	DirectX::SimpleMath::Vector3 target_dir = target_pos - pos;

	// X�����̉�]�𒲂ׂ�
	m_moveInfo.xVec = CheckVecX(dir, target_dir);
	// Y�����̉�]�𒲂ׂ�
	m_moveInfo.yVec = CheckVecY(dir, target_dir);

	// ���������Ȃ�߂��ꍇ
	if (distance < parameter.evadeNearBoostDistance) {
		m_moveInfo.useBoost = (status.sp >= parameter.evadeNearBoostSp);
	}
	// �������߂��ꍇ
	else if (distance < parameter.evadeFarBoostDistance) {
		m_moveInfo.useBoost = (status.sp >= parameter.evadeFarBoostSp);
	}
	// ����������ꍇ
	else {
		m_moveInfo.useBoost = false;
	}

}

