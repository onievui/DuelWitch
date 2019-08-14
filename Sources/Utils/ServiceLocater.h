#pragma once
#ifndef SERVICE_LOCATER_DEFINED
#define SERVICE_LOCATER_DEFINED


#include "ErrorMessage.h"


template <class T>
/// <summary>
/// �T�[�r�X���P�[�^
/// </summary>
class ServiceLocater {
public:
	// �T�[�r�X�̎擾
	static T* Get() {
		return s_getFunc();
	}

	// �T�[�r�X�̓o�^
	static void Register(T* service) {
		s_service = service;
		s_getFunc = (service ? &GetService : &NullService);
	}

	// �T�[�r�X�̉���
	static void Remove() {
		s_service = nullptr;
		s_getFunc = &NullService;
	}

private:
	// �T�[�r�X���o�^����Ă��Ȃ��ꍇ�̏���
	static T* NullService() {
		ErrorMessage(L"�T�[�r�X���o�^����Ă��܂���");
		return nullptr;
	}
	// �T�[�r�X�̎擾
	static T* GetService() {
		return s_service;
	}

private:
	// �T�[�r�X�ւ̃|�C���^
	static T*                  s_service;
	// �T�[�r�X���o�^�ς݂��ɉ����ăf�B�X�p�b�`
	static std::function<T*()> s_getFunc;

};

// �ÓI�����o�̏�����
template <class T>
T*                  ServiceLocater<T>::s_service = nullptr;
template <class T>
std::function<T*()> ServiceLocater<T>::s_getFunc = &ServiceLocater<T>::NullService;


#endif // !SERVICE_LOCATER_DEFINED