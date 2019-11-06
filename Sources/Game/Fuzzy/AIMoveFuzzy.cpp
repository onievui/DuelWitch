#include "AIMoveFuzzy.h"
#include <Utils\FuzzyUtils.h>


/// <summary>
/// ���͂���o�͂��v�Z����
/// </summary>
void AIMoveFuzzy::Execute() {
	// �t�@�W�[��
	// �ł��߂��G�v���C���[�Ƃ�HP�̍�
	float hpGap = Fuzzy::Grade(m_inputData.hpGap, -30.0f, 30.0f);
	//// SP
	//float sp = Fuzzy::Grade(m_inputData.sp, 0.0f, 100.0f);
	// �ł��߂��G�v���C���[�Ƃ̋���
	float distance = Fuzzy::Grade(m_inputData.distance, 3.0f, 60.0f);
	// �ł��߂��G�v���C���[�̕��������Ă��邩�ǂ���
	float lookingOther = Fuzzy::Bool(m_inputData.lookingOther, true);
	// �������Ă���G�������g�̐�
	float hasElementNum = Fuzzy::Grade(static_cast<float>(m_inputData.hasElementNum), 0.0f, 5.0f);
	// �ł��߂��G�������g�Ƃ̋���
	float elementDistance = Fuzzy::Grade(m_inputData.elementDistance, 3.0f, 60.0f);
	// �G�������g���t�B�[���h�ɑ��݂��邩�ǂ���
	float existElements = Fuzzy::Not(Fuzzy::Bool(m_inputData.elementDistance, 300.0f));

	// ��t�@�W�[��
	float collect_element_state = Fuzzy::And(Fuzzy::Not(hasElementNum), Fuzzy::Or(distance, Fuzzy::Not(elementDistance)));
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


