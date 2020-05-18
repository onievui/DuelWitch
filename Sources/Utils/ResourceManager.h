#pragma once
#ifndef RESOURCE_MANAGER_DEFINED
#define RESOURCE_MANAGER_DEFINED


#include <Utils\NonCopyable.h>
#include "Resource.h"
#include "ErrorMessage.h"


template <class T>
/// <summary>
/// ���\�[�X�}�l�[�W���N���X
/// </summary>
class ResourceManager final : NonCopyable {
public:
	using ResourceType = typename T::Type;
	using IDType       = typename T::IDType;

private:
	// �����[�h�̃��\�[�X�̃C���f�b�N�X
	static constexpr int NULL_INDEX = -1;

public:
	// �R���X�g���N�^
	ResourceManager(const std::wstring& kind) 
		: m_kind(kind) {
		Release();
		m_index.resize(static_cast<int>(IDType::Num), NULL_INDEX);
	}


	// �f�X�g���N�^
	~ResourceManager() {

	};

public:
	// ���\�[�X��ǉ�����
	bool AddResource(IDType id, T&& resource) {
		if (!resource.IsValid()) {
			std::wstring error_message = L"�̓ǂݍ��݂Ɏ��s���܂���";
			ErrorMessage((m_kind + error_message).c_str());
			return false;
		}
		m_index[static_cast<int>(id)] = m_resources.size();
		m_resources.emplace_back(std::make_unique<T>(std::forward<T>(resource)));
		return true;
	}

	template<class... Args>
	bool AddResource(IDType id, Args&&... args) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"�̎擾�Ɏ��s���܂���";
			ErrorMessage((m_kind + error_message).c_str());
			return false;
		}
		m_resources[index]->AddResource(args...);
		return true;
	}

	// ���\�[�X���������
	void Release() {
		m_resources.clear();
		m_resources.shrink_to_fit();
		std::fill(m_index.begin(), m_index.end(), NULL_INDEX);
	}

	// ���\�[�X���擾����
	const T* GetResource(IDType id) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"�̎擾�Ɏ��s���܂���";
			ErrorMessage((m_kind + error_message).c_str());
			return nullptr;
		}
		return m_resources[index].get();
	}

	// ��const�̃��\�[�X���擾����
	T* GetRawResource(IDType id) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"�̎擾�Ɏ��s���܂���";
			ErrorMessage((m_kind + error_message).c_str());
			return nullptr;
		}
		return m_resources[index].get();
	}

	// �S�Ẵ��\�[�X���擾����
	const std::vector<std::unique_ptr<T>>& GetAllResources() { return m_resources; }

	// ��const�̑S�Ẵ��\�[�X���擾����
	std::vector<std::unique_ptr<T>>& GetRawAllResources() { return m_resources; }

	// ���\�[�X��u��������
	void Replace(IDType id, ResourceType&& resource, int index2 = 0) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"�̎擾�Ɏ��s���܂���";
			ErrorMessage((m_kind + error_message).c_str());
			return;
		}
		T* data = m_resources[index].get();
		data->Replace(std::forward<ResourceType>(resource), index2);
	}

private:
	// ���\�[�X
	std::vector<std::unique_ptr<T>> m_resources;
	// ���\�[�X�̊i�[�ʒu
	std::vector<int>                m_index;
	// ���\�[�X�̎��
	const std::wstring              m_kind;

};


#endif // !RESOURCE_MANAGER_DEFINED