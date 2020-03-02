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
	float hp_gap = Fuzzy::Grade(m_inputData.hpGap, parameter.hpGapMin, parameter.hpGapMax);
	// 最も近い敵プレイヤーとの距離
	float distance = Fuzzy::Grade(m_inputData.distance, parameter.distanceMin, parameter.distanceMax);
	// 最も近い敵プレイヤーへの向き具合
	float looking_other = Fuzzy::Grade(m_inputData.lookingOther, std::cos(Math::PI), std::cos(Math::QuarterPI));
	// 所持しているエレメントの数
	float has_element_num = Fuzzy::Grade(static_cast<float>(m_inputData.hasElementNum), parameter.hasElementNumMin, parameter.hasElementNumMax);
	// エレメントを所持してないかどうか
	//float has_not_element = Fuzzy::ReverseGrade(static_cast<float>(m_inputData.hasElementNum), 0, 1);
	// 最も近いエレメントとの距離
	float element_distance = Fuzzy::Grade(m_inputData.elementDistance, parameter.elementDistanceMin, parameter.elementDistanceMax);
	// エレメントがフィールドに存在するかどうか
	float exist_elements = Fuzzy::Not(Fuzzy::Bool(m_inputData.elementDistance, 1000.0f));

	// ファジールールを適用する
	float collect_element_state = Fuzzy::Or(Fuzzy::Not(has_element_num), Fuzzy::Or(distance, Fuzzy::Not(element_distance)));
	//collect_element_state = Fuzzy::Or(collect_element_state, has_not_element);
	collect_element_state = Fuzzy::And(collect_element_state, exist_elements);
	float chase_state = Fuzzy::And(Fuzzy::Or(hp_gap, has_element_num), Fuzzy::And(Fuzzy::Not(distance), looking_other));
	float evade_state = Fuzzy::And(Fuzzy::Not(hp_gap), Fuzzy::And(Fuzzy::Not(distance), Fuzzy::Not(looking_other)));

	// 最大値を出力ステートとする
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


