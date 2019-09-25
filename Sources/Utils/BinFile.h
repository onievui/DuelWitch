#pragma once
#ifndef BIN_FILE_DEFINED
#define BIN_FILE_DEFINED


#include <fstream>


/// <summary>
/// �o�C�i���t�@�C���ǂݍ��݃N���X
/// </summary>
class BinFile {
public:
	// �R���X�g���N�^�i�t�@�C�������w�肵�ēǂݍ��݁j
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
	// �f�[�^���擾����
	const void* Data() const { return m_data.get(); }
	// �f�[�^�T�C�Y���擾����
	size_t Size() const { return m_size; }

private:
	// �f�[�^�T�C�Y
	size_t m_size;
	// �f�[�^�{��
	std::unique_ptr<char> m_data;
};

#endif // !BIN_FILE_DEFINED
