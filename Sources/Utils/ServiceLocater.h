#pragma once
#ifndef SERVICE_LOCATER_DEFINED
#define SERVICE_LOCATER_DEFINED


#include "ErrorMessage.h"


template <class T>
/// <summary>
/// サービスロケータ
/// </summary>
class ServiceLocater {
public:
	// サービスを取得する
	static T* Get() {
		return s_getFunc();
	}

	// サービスを登録する
	static void Register(T* service) {
		s_service = service;
		s_getFunc = (service ? &GetService : &NullService);
	}

	// サービスを解除する
	static void Unregister() {
		s_service = nullptr;
		s_getFunc = &NullService;
	}

private:
	// サービスが登録されていない場合の処理
	static T* NullService() {
		ErrorMessage(L"サービスが登録されていません");
		return nullptr;
	}
	// サービスの取得
	static T* GetService() {
		return s_service;
	}

private:
	// サービスへのポインタ
	static T*                  s_service;
	// サービスが登録済みかに応じてディスパッチ
	static std::function<T*()> s_getFunc;

};

// 静的メンバを初期化する
template <class T>
T*                  ServiceLocater<T>::s_service = nullptr;
template <class T>
std::function<T*()> ServiceLocater<T>::s_getFunc = &ServiceLocater<T>::NullService;


#endif // !SERVICE_LOCATER_DEFINED
