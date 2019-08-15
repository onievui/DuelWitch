#pragma once
#ifndef SINGLETON_DEFINED
#define SINGLETON_DEFINED


#include <functional>


template <typename T>
/// <summary>
/// シングルトンクラステンプレート
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
	// インスタンスを取得する
	static T* GetIns() {
		return s_getFunc();
	}

	// インスタンスを解放する
	static void Dispose() {
		s_instance.reset(nullptr);
		s_getFunc = &InitializeGetIns;
	}

private:
	// 初期化してから取得する
	static T* InitializeGetIns() {
		s_instance = std::make_unique<T>();
		s_getFunc = &NormalGetIns;
		return s_instance.get();
	}

	// そのまま取得する
	static T* NormalGetIns() {
		return s_instance.get();
	}

private:
	// インスタンスへの静的ポインタ
	static std::unique_ptr<T>  s_instance;
	// インスタンスが生成済みかに応じてディスパッチ
	static std::function<T*()> s_getFunc;
};

// 静的メンバを初期化する
template <class T>
std::unique_ptr<T>  Singleton<T>::s_instance = nullptr;
template <class T>
std::function<T*()> Singleton<T>::s_getFunc = &Singleton<T>::InitializeGetIns;


#endif // !SINGLETON_DEFINED
