#pragma once
#ifndef LAMDA_UTILS_DEFINED
#define LAMDA_UTILS_DEFINED


template<class T>
class is_mem_func;


/// <summary>
/// ラムダ関係クラス
/// </summary>
class LamdaUtils {
public:
	// nullチェックラムダ
	static auto NotNull() {
		return [](auto* object) { return object != nullptr; };
	}

	// nullチェックラムダ
	static auto IsNull() {
		return [](auto* object) { return object == nullptr; };
	}

	// std::find_ifの簡易版
	static auto FindIf() {
		return [](auto& container, auto pred) {return std::find_if(container.begin(), container.end(), pred); };
	}

	template<class T, class U>
	// std::find_ifの簡易版
	static auto FindIf(T& container, U pred) {
		return std::find_if(container.begin(), container.end(), pred);
	}

	// std::find_ifを続ける
	template<class T, class U>
	static void FindIfNext(T& itr, const T& end, U pred) {
		itr = std::find_if(itr + 1, end, pred);
	}

public:
	template<class T>
	// ラムダを取得する（メンバ関数）
	static auto GetLamda(T f) -> std::enable_if_t<is_mem_func<T>::value, decltype(std::mem_fn(f))> {
		return std::mem_fn(f);
	}

	template<class T>
	// ラムダを取得する（非メンバ関数）
	static auto GetLamda(T f) ->std::enable_if_t<!is_mem_func<T>::value, decltype(f)> {
		return f;
	}
};


template<typename T>
/// <summary>
/// メンバ関数か判定するヘルパクラス
/// </summary>
class is_mem_func {
private:
	template<typename U>
	static auto Check(U v)->decltype(std::mem_fn(v), std::true_type()) {};
	static auto Check(...)->decltype(std::false_type()) {};
public:
	typedef decltype(Check(std::declval<T>())) type;
	static bool const value = type::value;
};


#endif // !LAMDA_UTILS_DEFINED
