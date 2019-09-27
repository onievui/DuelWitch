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
	// �����̎擾�i�����j
	static int GetRand(int range);
	// �����̎擾�i�����j
	static float GetRand(float range);
	// �͈͂̒��̗����̎擾�i�����j
	static int GetRange(int min, int max);
	// �͈͂̒��̗����̎擾�i�����j
	static float GetRange(float min, float max);

private:
	static RandMt m_randMt;
	static std::random_device  m_seed;
	static std::mt19937        m_mt;
};


#endif // !RANDMT_DEFINED