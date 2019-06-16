#include "RandMt.h"

std::mt19937 RandMt::m_mt;

/// <summary>
/// コンストラクタ
/// </summary>
RandMt::RandMt() {
	m_mt.seed(m_seed());
}

/// <summary>
/// 乱数の取得
/// </summary>
/// <param name="_range">範囲（整数値）</param>
/// <returns>
/// 0~整数値-1までの乱数
/// </returns>
int RandMt::GetRand(const int _range) {
	static RandMt instance;
	return m_mt() % _range;
}
