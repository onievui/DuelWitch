#pragma once
#ifndef COMMON_SERVICES_DEFINED
#define COMMON_SERVICES_DEFINED


#include <Utils\ServiceLocater.h>


template <class T>
class ResourceManager;


/// <summary>
/// ��{�I�ȃT�[�r�X���܂Ƃ߂��N���X
/// </summary>
class CommonServices {
public:
	// �R���X�g���N�^
	CommonServices();
	// �f�X�g���N�^
	~CommonServices();
	// ���[�u�R���X�g���N�^
	CommonServices(CommonServices&& from);
	// ���[�u������Z�q�̃I�[�o�[���[�h
	CommonServices& operator=(CommonServices&& from);

public:
	// �R�����T�[�r�X������������
	void Initialize();
	// �I���������s��
	void Finalize();

private:
	template<class T, class... Args>
	// �T�[�r�X�𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	void RegisterService(std::unique_ptr<T>& service, Args&&... args);

	template<class T>
	// ���\�[�X�}�l�[�W���𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	void RegisterResourceManager(std::unique_ptr<ResourceManager<T>>& manager, const std::wstring& kind);

private:
	// �T�[�r�X�ێ��N���X
	class Services;
	std::unique_ptr<Services> m_services;

};


#endif // !COMMON_SERVICES_DEFINED
