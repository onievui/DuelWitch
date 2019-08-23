#include "JsonWrapper.h"
#include <fstream>


namespace JsonWrapper {

	/// <summary>
	/// JSON�f�[�^���o�͂���
	/// </summary>
	/// <param name="os">�o�͐�X�g���[��</param>
	/// <param name="value">JSON�f�[�^</param>
	/// <returns>
	/// �o�͐�X�g���[��
	/// </returns>
	std::ostream &operator << (std::ostream& os, JsonWrapper::value &value) {
		value.getValue().serialize(std::ostream_iterator<char>(os));
		return os;
	}

	/// <summary>
	/// JSON�f�[�^��ǂݍ���
	/// </summary>
	/// <param name="is">���͌��X�g���[��</param>
	/// <param name="value">JSON�f�[�^</param>
	/// <returns>
	/// ���͌��X�g���[��
	/// </returns>
	std::istream &operator >> (std::istream& is, JsonWrapper::value& value) {
		picojson::set_last_error(std::string());
		const std::string err(picojson::parse(value.getValue(), is));
		if (!err.empty()) {
			picojson::set_last_error(err);
			is.setstate(std::ios::failbit);
		}
		return is;
	}

	/// <summary>
	/// �t�@�C������JSON�f�[�^��ǂݍ���
	/// </summary>
	/// <param name="value"></param>
	/// <param name="fileName"></param>
	/// <returns>
	/// true  : ����
	/// false : ���s
	/// </returns>
	bool LoadCheck(JsonWrapper::value& value, const std::wstring& fileName) {
		std::ifstream ifs;
		ifs.open(fileName.c_str());
		if (!ifs.is_open()) {
			return false;
		}

		ifs >> value;
		ifs.close();

		return true;
	}
}