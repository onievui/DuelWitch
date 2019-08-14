#pragma once
#ifndef SINGLETON_DEFINED
#define SINGLETON_DEFINED


#include <functional>


template <typename T>
/// <summary>
/// �V���O���g���N���X�e���v���[�g
/// </summary>
class Singleton {
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

public:
	// �C���X�^���X�̎擾
	static T* GetIns() {
		return s_getFunc();
	}

	// �C���X�^���X�̉��
	static void Dispose() {
		s_instance.reset(nullptr);
		s_getFunc = &InitializeGetIns;
	}

private:
	// ���������Ă���擾
	static T* InitializeGetIns() {
		s_instance = std::make_unique<T>();
		s_getFunc = &NormalGetIns;
		return s_instance.get();
	}

	// ���̂܂܎擾
	static T* NormalGetIns() {
		return s_instance.get();
	}

private:
	// �C���X�^���X�ւ̐ÓI�|�C���^
	static std::unique_ptr<T>  s_instance;
	// �C���X�^���X�������ς݂��ɉ����ăf�B�X�p�b�`
	static std::function<T*()> s_getFunc;
};

// �ÓI�����o�̏�����
template <class T>
std::unique_ptr<T>  Singleton<T>::s_instance = nullptr;
template <class T>
std::function<T*()> Singleton<T>::s_getFunc = &Singleton<T>::InitializeGetIns;


#endif // !SINGLETON_DEFINED
