#pragma once
#ifndef JSON_WRAPPER_DEFINED
#define JSON_WRAPPER_DEFINED


#include "picojson.h"


namespace JsonWrapper {

	/// <summary>
	/// JSONデータクラス
	/// </summary>
	class value {
	public:
		// コンストラクタ
		value(picojson::value& val)
			: m_val(val) {
		}

		// 要素にアクセスする
		JsonWrapper::value operator[](const char* name) {
			return JsonWrapper::value(m_val.get<picojson::object>()[name]);
		}

		// 配列にアクセスする
		JsonWrapper::value operator[](int index) {
			return JsonWrapper::value(m_val.get<picojson::array>()[index]);
		}

		template <class T>
		// データを取得する
		T& get() {
			return m_val.get<T>();
		}

		// データを取得する
		int getNumI() {
			return static_cast<int>(m_val.get<double>());
		}

		//データを取得する
		float getNumF() {
			return static_cast<float>(m_val.get<double>());
		}

		template <class T>
		// データを設定する
		void set(const T& val) {
			m_val = picojson::value(val);
		}

		// picojsonオブジェクトを取得する
		picojson::value& getValue() {
			return m_val;
		}

		// シリアライズを行う
		std::string serialize(bool prettify) const {
			return m_val.serialize(prettify);
		}

	private:
		// 元データへの参照
		picojson::value& m_val;
	};


	/// <summary>
	/// JSONデータクラス（ルート用）
	/// </summary>
	class root : public JsonWrapper::value {
	public:
		// コンストラクタ
		root()
			: JsonWrapper::value(m_root) {
		}

	private:
		// 元データ
		picojson::value m_root;
	};

	// JSONデータを出力する
	extern std::ostream &operator << (std::ostream& os, JsonWrapper::value &value);

	// JSONデータを読み込む
	extern std::istream &operator >> (std::istream& is, JsonWrapper::value& value);

	// ファイルからJSONデータを読み込む
	extern bool LoadCheck(JsonWrapper::value& value, const std::wstring& fileName);

}

#endif // !JSON_WRAPPER_DEFINED

