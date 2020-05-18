#pragma once
#ifndef LOAD_DATA_HOLDER_DEFINED
#define LOAD_DATA_HOLDER_DEFINED


#include "ILoadDataHolder.h"
#include "LoadDataManager.h"


template<class T, LoadDataID DataID>
/// <summary>
/// �ǂݍ��݃f�[�^�ێ��N���X
/// </summary>
class LoadDataHolder : public ILoadDataHolder {
	friend class LoadDataManager;

public:
	// �R�Â���ǂݍ��݃f�[�^ID
	static constexpr LoadDataID ID = DataID;

public:
	// �R���X�g���N�^
	LoadDataHolder()
		: m_data() {
		// LoadDataManager::GetIns()->Regiser(this);
	}
	// �f�X�g���N�^
	~LoadDataHolder() {
		// LoadDataManager::GetIns()->Unregister(this);
	}

private:
	// �f�[�^��ǂݍ���
	bool Load() override {
		if (!m_data) {
			m_data = std::make_unique<T>();
		}
		return m_data->Load();
	}

	// �f�[�^���ēǂݍ��݂���
	bool Reload() override {
		return m_data->Reload();
	}

	// �f�[�^���������
	void Dispose() override {
		m_data.reset();
	}

	// �ǂݍ��݃f�[�^ID���擾����
	LoadDataID GetID() const override {
		return ID;
	}

public:
	// �f�[�^���擾����
	const T* Get() const {
		return m_data.get();
	}

public:
	T* operator->() {
		return m_data.get();
	}

private:
	// �ǂݍ��ރf�[�^�{��
	std::unique_ptr<T> m_data;

};


#endif // !LOAD_DATA_HOLDER_DEFINED
