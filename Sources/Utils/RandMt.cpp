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
	return std::uniform_int_distribution<>(0, range-1)(m_mt);
}

/// <summary>
/// �����̎擾�i�����j
/// </summary>
/// <param name="range">�͈́i�����j</param>
/// <returns>
/// 0~�����l�܂ł̗���
/// </returns>
float RandMt::GetRand(float range) {
	return static_cast<float>(std::uniform_real_distribution<>(0.0f, range)(m_mt));
}

/// <summary>
/// �͈͂̒��̗����̎擾�i�����j
/// </summary>
/// <param name="min">����</param>
/// <param name="max">���</param>
/// <returns>
/// ����~����܂ł̗���
/// </returns>
int RandMt::GetRange(int min, int max) {
	return std::uniform_int_distribution<>(min, max)(m_mt);
}

/// <summary>
/// �͈͂̒��̗����̎擾�i�����j
/// </summary>
/// <param name="min">����</param>
/// <param name="max">���</param>
/// <returns>
/// ����~����܂ł̗���
/// </returns>
float RandMt::GetRange(float min, float max) {
	return static_cast<float>(std::uniform_real_distribution<>(min, max)(m_mt));
}
