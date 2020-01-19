#include "AIMoveFuzzy.h"
#include <Utils\FuzzyUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Parameters\FuzzyParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// 入力から出力を計算する
/// </summary>
void AIMoveFuzzy::Execute() {
	const FuzzyParameter::ai_move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFuzzyParameter()->aiMoveParam;

	// ファジー化
	// 最も近い敵プレイヤーとのHPの差
	float hpGap = Fuzzy::Grade(m_inputData.hpGap, parameter.hpGapMin, parameter.hpGapMax);
	//// SP
	//float sp = Fuzzy::Grade(m_inputData.sp, 0.0f, 100.0f);
	// 最も近い敵プレイヤーとの距離
	float distance = Fuzzy::Grade(m_inputData.distance, parameter.distanceMin, parameter.distanceMax);
	// 最も近い敵プレイヤーの方を向いているかどうか
	//float lookingOther = Fuzzy::Bool(m_inputData.lookingOther, true);
	// 最も近い敵プレイヤーへの向き具合
	float lookingOther = Fuzzy::Grade(m_inputData.lookingOther, std::cos(Math::PI), std::cos(Math::QuarterPI));
	// 所持しているエレメントの数
	float hasElementNum = Fuzzy::Grade(static_cast<float>(m_inputData.hasElementNum), parameter.hasElementNumMin, parameter.hasElementNumMax);
	// エレメントを所持してないかどうか
	float hasNotElement = Fuzzy::ReverseGrade(static_cast<float>(m_inputData.hasElementNum), 0, 1);
	// 最も近いエレメントとの距離
	float elementDistance = Fuzzy::Grade(m_inputData.elementDistance, parameter.elementDistanceMin, parameter.elementDistanceMax);
	// エレメントがフィールドに存在するかどうか
	float existElements = Fuzzy::Not(Fuzzy::Bool(m_inputData.elementDistance, 1000.0f));

	// 非ファジー化
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


