#pragma once
#ifndef RANDMT_DEFINED
#define RANDMT_DEFINED


#include <random>


/// <summary>
///	���������N���X
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
	// �����̎擾
	static int GetRand(const int _range);

private:
	std::random_device  m_seed;
	static std::mt19937 m_mt;
};


#endif // !RANDMT_DEFINED