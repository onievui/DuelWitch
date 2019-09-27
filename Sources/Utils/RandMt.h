#pragma once
#ifndef RANDMT_DEFINED
#define RANDMT_DEFINED


#include <random>


/// <summary>
///	乱数生成クラス
/// </summary>
class RandMt {
private:
	RandMt();
	~RandMt() = default;

public:
	RandMt(const RandMt&) = delete;
	RandMt& operator=(const RandMt&) = delete;
	RandMt(RandMt&&) = delete;
	RandMt& operator=(RandMt&&) = delete;

public:
	// 乱数の取得（整数）
	static int GetRand(int range);
	// 乱数の取得（実数）
	static float GetRand(float range);
	// 範囲の中の乱数の取得（整数）
	static int GetRange(int min, int max);
	// 範囲の中の乱数の取得（実数）
	static float GetRange(float min, float max);

private:
	static RandMt m_randMt;
	static std::random_device  m_seed;
	static std::mt19937        m_mt;
};


#endif // !RANDMT_DEFINED