#include "RandMt.h"


std::mt19937       RandMt::m_mt;
std::random_device RandMt::m_seed;
RandMt             RandMt::m_randMt;


/// <summary>
/// �R���X�g���N�^
/// </summary>
RandMt::RandMt() {
	m_mt.seed(m_seed());
}

/// <summary>
/// �����̎擾�i�����j
/// </summary>
/// <param name="_range">�͈́i�����l�j</param>
/// <returns>
/// 0~�����l-1�܂ł̗���
/// </returns>
int RandMt::GetRand(const int range) {
	return m_mt() % range;
}

/// <summary>
/// �����̎擾�i�����j
/// </summary>
/// <param name="range">�͈́i�����j</param>
/// <returns>
/// 0~�����l�܂ł̗���
/// </returns>
float RandMt::GetRand(float range) {
	return GetRand(10001)/10000.0f*range;
}
