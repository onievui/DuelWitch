#include "JsonWrapper.h"
#include <fstream>
#include "ErrorMessage.h"


namespace JsonWrapper {

	/// <summary>
	/// JSONデータを出力する
	/// </summary>
	/// <param name="os">出力先ストリーム</param>
	/// <param name="value">JSONデータ</param>
	/// <returns>
	/// 出力先ストリーム
	/// </returns>
	std::ostream &operator << (std::ostream& os, JsonWrapper::value &value) {
		value.GetValue().serialize(std::ostream_iterator<char>(os));
		return os;
	}

	/// <summary>
	/// JSONデータを読み込む
	/// </summary>
	/// <param name="is">入力元ストリーム</param>
	/// <param name="value">JSONデータ</param>
	/// <returns>
	/// 入力元ストリーム
	/// </returns>
	std::istream &operator >> (std::istream& is, JsonWrapper::value& value) {
		picojson::set_last_error(std::string());
		const std::string err(picojson::parse(value.GetValue(), is));
		if (!err.empty()) {
			picojson::set_last_error(err);
			is.setstate(std::ios::failbit);
		}
		return is;
	}

	/// <summary>
	/// ファイルからJSONデータを読み込む
	/// </summary>
	/// <param name="value"></param>
	/// <param name="fileName"></param>
	/// <returns>
	/// true  : 成功
	/// false : 失敗
	/// </returns>
	bool LoadCheck(JsonWrapper::value& value, const std::wstring& fileName) {
		std::ifstream ifs;
		ifs.open(fileName.c_str());
		if (!ifs.is_open()) {
			ErrorMessage(L"Jsonファイルの読み込みに失敗しました");
			return false;
		}

		ifs >> value;
		ifs.close();

		return true;
	}
}