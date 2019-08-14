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
	// サービスの取得
	static T* Get() {
		return s_getFunc();
	}

	// サービスの登録
	static void Register(T* service) {
		s_service = service;
		s_getFunc = (service ? &GetService : &NullService);
	}

	// サービスの解除
	static void Remove() {
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

// 静的メンバの初期化
template <class T>
T*                  ServiceLocater<T>::s_service = nullptr;
template <class T>
std::function<T*()> ServiceLocater<T>::s_getFunc = &ServiceLocater<T>::NullService;


#endif // !SERVICE_LOCATER_DEFINED