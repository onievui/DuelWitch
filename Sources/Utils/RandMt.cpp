#include "RandMt.h"


std::mt19937       RandMt::m_mt;
std::random_device RandMt::m_seed;
RandMt             RandMt::m_randMt;


/// <summary>
/// コンストラクタ
/// </summary>
RandMt::RandMt() {
	m_mt.seed(m_seed());
}

/// <summary>
/// 乱数の取得（整数）
/// </summary>
/// <param name="_range">範囲（整数値）</param>
/// <returns>
/// 0~整数値-1までの乱数
/// </returns>
int RandMt::GetRand(const int range) {
	return m_mt() % range;
}

/// <summary>
/// 乱数の取得（実数）
/// </summary>
/// <param name="range">範囲（実数）</param>
/// <returns>
/// 0~実数値までの乱数
/// </returns>
float RandMt::GetRand(float range) {
	return GetRand(10001)/10000.0f*range;
}
