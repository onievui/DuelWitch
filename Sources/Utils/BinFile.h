#pragma once
#ifndef BIN_FILE_DEFINED
#define BIN_FILE_DEFINED


#include <fstream>


/// <summary>
/// バイナリファイル読み込みクラス
/// </summary>
class BinFile {
public:
	// コンストラクタ（ファイル名を指定して読み込み）
	BinFile(const wchar_t* fileName) {
		m_size = 0;
		std::ifstream binfile(fileName, std::ios::in | std::ios::binary);

		if (binfile.is_open()) {
			size_t data_size = static_cast<size_t>(binfile.seekg(0, std::ios::end).tellg());
			binfile.seekg(0, std::ios::beg);
			std::unique_ptr<char> code(new char[data_size]);
			binfile.read(code.get(), data_size);
			m_size = data_size;
			m_data = std::move(code);
		}
	}

public:
	// データを取得する
	const void* Data() const { return m_data.get(); }
	// データサイズを取得する
	size_t Size() const { return m_size; }

private:
	// データサイズ
	size_t m_size;
	// データ本体
	std::unique_ptr<char> m_data;
};

#endif // !BIN_FILE_DEFINED
