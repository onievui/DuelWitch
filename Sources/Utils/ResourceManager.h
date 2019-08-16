#pragma once
#ifndef RESOURCE_MANAGER_DEFINED
#define RESOURCE_MANAGER_DEFINED


#include <Utils\NonCopyable.h>
#include "Resource.h"


template <class T>
class ResourceManager final : NonCopyable {
public:
	using IDType = typename T::IDType;

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

	// ���\�[�X���J������
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

private:
	// ���\�[�X
	std::vector<std::unique_ptr<T>> m_resources;
	// ���\�[�X�̊i�[�ʒu
	std::vector<int>                m_index;
	// ���\�[�X�̎��
	std::wstring                    m_kind;

};


///// <summary>
///// ���\�[�X�Ǘ��N���X
///// </summary>
//class ResourceManager final : NonCopyable {
//private:
//	// �����[�h�̃��\�[�X�̃C���f�b�N�X
//	static constexpr int NULL_INDEX = -1;
//
//public:
//	// �R���X�g���N�^
//	ResourceManager();
//	// �f�X�g���N�^
//	~ResourceManager();
//
//public:
//	// ���\�[�X�����[�h����
//	void Load();
//	// ���\�[�X���J������
//	void Release();
//
//	// �e�N�X�`�����\�[�X���擾����
//	std::shared_ptr<TextureResource> GetTexture(TextureID id);
//	// �t�H���g���\�[�X���擾����
//	std::shared_ptr<FontResource> GetFont(FontID id);
//
//private:
//	// �e�N�X�`�����\�[�X��ǉ�����
//	bool AddTexture(TextureID id, std::shared_ptr<TextureResource>& texture);
//	// �t�H���g���\�[�X��ǉ�����
//	bool AddFont(FontID id, std::shared_ptr<FontResource>& font);
//	template <class T>
//	// ���\�[�X��ǉ�����
//	void AddResource(std::shared_ptr<T>& resource, int id, std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
//		index[id] = resources.size();
//		resources.emplace_back(resource);
//	}
//
//	template <class T>
//	// �L���ȃ��\�[�X���m�F����
//	bool CheckIsValid(const std::shared_ptr<T>& resource) {
//		return resource->IsValid();
//	}
//
//	template <class T>
//	// ���\�[�X�����Z�b�g����
//	void Reset(std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
//		resources.clear();
//		resources.shrink_to_fit();
//		std::fill(index.begin(), index.end(), NULL_INDEX);
//	}
//
//private:
//	// �e�N�X�`�����\�[�X
//	std::vector<std::shared_ptr<TextureResource>>     m_textures;
//	// �e�N�X�`�����\�[�X�̊i�[�ʒu
//	std::vector<int>                                  m_texturesIndex;
//	// �t�H���g���\�[�X
//	std::vector<std::shared_ptr<FontResource>>        m_fonts;
//	// �t�H���g���\�[�X�̊i�[�ʒu
//	std::vector<int>                                  m_fontsIndex;
//
//};


#endif // !RESOURCE_MANAGER_DEFINED