#pragma once
#ifndef IOBSERVER_DEFINED
#define IOBSERVER_DEFINED


class ISubject;


/// <summary>
/// �I�u�U�[�o�C���^�t�F�[�X�N���X
/// </summary>
class IObserver {
public:
	// �I�u�U�[�o���X�V����
	virtual void Update(ISubject* subject) = 0;
};


#endif	// !IOBSERVER_DEFINED
