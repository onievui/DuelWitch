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
	return std::uniform_int_distribution<>(0, range-1)(m_mt);
}

/// <summary>
/// 乱数の取得（実数）
/// </summary>
/// <param name="range">範囲（実数）</param>
/// <returns>
/// 0~実数値までの乱数
/// </returns>
float RandMt::GetRand(float range) {
	return static_cast<float>(std::uniform_real_distribution<>(0.0f, range)(m_mt));
}

/// <summary>
/// 範囲の中の乱数の取得（整数）
/// </summary>
/// <param name="min">下限</param>
/// <param name="max">上限</param>
/// <returns>
/// 下限~上限までの乱数
/// </returns>
int RandMt::GetRange(int min, int max) {
	return std::uniform_int_distribution<>(min, max)(m_mt);
}

/// <summary>
/// 範囲の中の乱数の取得（実数）
/// </summary>
/// <param name="min">下限</param>
/// <param name="max">上限</param>
/// <returns>
/// 下限~上限までの乱数
/// </returns>
float RandMt::GetRange(float min, float max) {
	return static_cast<float>(std::uniform_real_distribution<>(min, max)(m_mt));
}
