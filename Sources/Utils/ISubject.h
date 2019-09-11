#pragma once
#ifndef ISUBJECT_DEFINED
#define ISUBJECT_DEFINED


class IObserver;


/// <summary>
/// �T�u�W�F�N�g�C���^�t�F�[�X�N���X
/// </summary>
class ISubject {
public:
	// �I�u�U�[�o���A�^�b�`����
	virtual void Attach(IObserver* observer) = 0;
	// �I�u�U�[�o���f�^�b�`����
	virtual void Detach(IObserver* observer) = 0;
	// �I�u�U�[�o�ɒʒm����
	virtual void Notify() = 0;

};


#endif	// !ISUBJECT_DEFINED
