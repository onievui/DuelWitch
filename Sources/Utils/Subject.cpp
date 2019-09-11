#include "Subject.h"
#include "IObserver.h"
#include "ErrorMessage.h"


/// <summary>
/// �I�u�U�[�o���A�^�b�`����
/// </summary>
/// <param name="observer">�I�u�U�[�o</param>
void Subject::Attach(IObserver* observer) {
	if (!observer) {
		ErrorMessage(L"�T�u�W�F�N�g�ɖ����Ȓl���A�^�b�`�o���܂���");
		return;
	}
	m_list.push_back(observer);
}

/// <summary>
/// �I�u�U�[�o���f�^�b�`����
/// </summary>
/// <param name="observer">�I�u�U�[�o</param>
void Subject::Detach(IObserver* observer) {
	m_list.erase(std::remove(m_list.begin(), m_list.end(), observer), m_list.end());
}

/// <summary>
/// �I�u�U�[�o�ɒʒm����
/// </summary>
void Subject::Notify() {
	for (std::vector<IObserver*>::iterator itr = m_list.begin(); itr != m_list.end(); itr++) {
		(*itr)->Update(this);
	}
}
