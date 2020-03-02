#include "AIMoveFuzzy.h"
#include <Utils\FuzzyUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Parameters\FuzzyParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// ���͂���o�͂��v�Z����
/// </summary>
void AIMoveFuzzy::Execute() {
	const FuzzyParameter::ai_move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFuzzyParameter()->aiMoveParam;

	// �t�@�W�[��
	// �ł��߂��G�v���C���[�Ƃ�HP�̍�
	float hp_gap = Fuzzy::Grade(m_inputData.hpGap, parameter.hpGapMin, parameter.hpGapMax);
	// �ł��߂��G�v���C���[�Ƃ̋���
	float distance = Fuzzy::Grade(m_inputData.distance, parameter.distanceMin, parameter.distanceMax);
	// �ł��߂��G�v���C���[�ւ̌����
	float looking_other = Fuzzy::Grade(m_inputData.lookingOther, std::cos(Math::PI), std::cos(Math::QuarterPI));
	// �������Ă���G�������g�̐�
	float has_element_num = Fuzzy::Grade(static_cast<float>(m_inputData.hasElementNum), parameter.hasElementNumMin, parameter.hasElementNumMax);
	// �G�������g���������ĂȂ����ǂ���
	//float has_not_element = Fuzzy::ReverseGrade(static_cast<float>(m_inputData.hasElementNum), 0, 1);
	// �ł��߂��G�������g�Ƃ̋���
	float element_distance = Fuzzy::Grade(m_inputData.elementDistance, parameter.elementDistanceMin, parameter.elementDistanceMax);
	// �G�������g���t�B�[���h�ɑ��݂��邩�ǂ���
	float exist_elements = Fuzzy::Not(Fuzzy::Bool(m_inputData.elementDistance, 1000.0f));

	// �t�@�W�[���[����K�p����
	float collect_element_state = Fuzzy::Or(Fuzzy::Not(has_element_num), Fuzzy::Or(distance, Fuzzy::Not(element_distance)));
	//collect_element_state = Fuzzy::Or(collect_element_state, has_not_element);
	collect_element_state = Fuzzy::And(collect_element_state, exist_elements);
	float chase_state = Fuzzy::And(Fuzzy::Or(hp_gap, has_element_num), Fuzzy::And(Fuzzy::Not(distance), looking_other));
	float evade_state = Fuzzy::And(Fuzzy::Not(hp_gap), Fuzzy::And(Fuzzy::Not(distance), Fuzzy::Not(looking_other)));

	// �ő�l���o�̓X�e�[�g�Ƃ���
	float state_max = std::max({ collect_element_state, chase_state, evade_state });
	if (Math::Equal0(state_max - collect_element_state)) {
		m_outputData.state = AIMoveOutputCrisp::State::CollectElement;
	}
	else if (Math::Equal0(state_max - chase_state)) {
		m_outputData.state = AIMoveOutputCrisp::State::Chase;
	}
	else {
		m_outputData.state = AIMoveOutputCrisp::State::Evade;
	}

}


