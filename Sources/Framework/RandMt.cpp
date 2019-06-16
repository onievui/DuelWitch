#include "RandMt.h"

std::mt19937 RandMt::m_mt;

/// <summary>
/// �R���X�g���N�^
/// </summary>
RandMt::RandMt() {
	m_mt.seed(m_seed());
}

/// <summary>
/// �����̎擾
/// </summary>
/// <param name="_range">�͈́i�����l�j</param>
/// <returns>
/// 0~�����l-1�܂ł̗���
/// </returns>
int RandMt::GetRand(const int _range) {
	static RandMt instance;
	return m_mt() % _range;
}
