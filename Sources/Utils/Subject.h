#pragma once
#ifndef SUBJECT_DEFINED
#define SUBJECT_DEFINED


#include "ISubject.h"


class IObserver;


/// <summary>
/// �T�u�W�F�N�g�N���X
/// </summary>
class Subject : public ISubject {
public:
	// �I�u�U�[�o���A�^�b�`����
	void Attach(IObserver* observer) override;
	// �I�u�U�[�o���f�^�b�`����
	void Detach(IObserver* observer) override;
	// �I�u�U�[�o�ɒʒm����
	void Notify() override;

private:
	// �A�^�b�`���Ă���I�u�U�[�o�̃��X�g
	std::vector<IObserver*> m_list;
};


#endif	// !SUBJECT_DEFINED
