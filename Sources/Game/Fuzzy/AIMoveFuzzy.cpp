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
	float hpGap = Fuzzy::Grade(m_inputData.hpGap, parameter.hpGapMin, parameter.hpGapMax);
	//// SP
	//float sp = Fuzzy::Grade(m_inputData.sp, 0.0f, 100.0f);
	// �ł��߂��G�v���C���[�Ƃ̋���
	float distance = Fuzzy::Grade(m_inputData.distance, parameter.distanceMin, parameter.distanceMax);
	// �ł��߂��G�v���C���[�̕��������Ă��邩�ǂ���
	//float lookingOther = Fuzzy::Bool(m_inputData.lookingOther, true);
	// �ł��߂��G�v���C���[�ւ̌����
	float lookingOther = Fuzzy::Grade(m_inputData.lookingOther, std::cos(Math::PI), std::cos(Math::QuarterPI));
	// �������Ă���G�������g�̐�
	float hasElementNum = Fuzzy::Grade(static_cast<float>(m_inputData.hasElementNum), parameter.hasElementNumMin, parameter.hasElementNumMax);
	// �G�������g���������ĂȂ����ǂ���
	float hasNotElement = Fuzzy::ReverseGrade(static_cast<float>(m_inputData.hasElementNum), 0, 1);
	// �ł��߂��G�������g�Ƃ̋���
	float elementDistance = Fuzzy::Grade(m_inputData.elementDistance, parameter.elementDistanceMin, parameter.elementDistanceMax);
	// �G�������g���t�B�[���h�ɑ��݂��邩�ǂ���
	float existElements = Fuzzy::Not(Fuzzy::Bool(m_inputData.elementDistance, 1000.0f));

	// ��t�@�W�[��
	float collect_element_state = Fuzzy::And(Fuzzy::Not(hasElementNum), Fuzzy::Or(distance, Fuzzy::Not(elementDistance)));
	collect_element_state = Fuzzy::Or(collect_element_state, hasNotElement);
	collect_element_state = Fuzzy::And(collect_element_state, existElements);
	float chase_state = Fuzzy::And(Fuzzy::Or(hpGap, hasElementNum), Fuzzy::And(Fuzzy::Not(distance), lookingOther));
	float evade_state = Fuzzy::And(Fuzzy::Not(hpGap), Fuzzy::And(Fuzzy::Not(distance), Fuzzy::Not(lookingOther)));

	if (chase_state >= collect_element_state) {
		if (chase_state >= evade_state) {
			m_outputData.state = AIMoveOutputCrisp::State::Chase;
		}
		else {
			m_outputData.state = AIMoveOutputCrisp::State::Evade;
		}
	}
	else if (evade_state >= collect_element_state) {
		m_outputData.state = AIMoveOutputCrisp::State::Evade;
	}
	else {
		m_outputData.state = AIMoveOutputCrisp::State::CollectElement;
	}

}


