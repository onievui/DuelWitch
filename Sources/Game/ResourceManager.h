#pragma once
#ifndef RESOURCE_MANAGER_DEFINED
#define RESOURCE_MANAGER_DEFINED


#include <Utils\NonCopyable.h>
#include <Utils\Resource.h>
#include "ResourceID.h"


/// <summary>
/// ���\�[�X�Ǘ��N���X
/// </summary>
class ResourceManager final : NonCopyable {
private:
	// �����[�h�̃��\�[�X�̃C���f�b�N�X
	static constexpr int NULL_INDEX = -1;

public:
	// �R���X�g���N�^
	ResourceManager();
	// �f�X�g���N�^
	~ResourceManager();

public:
	// ���\�[�X�����[�h����
	void Load();
	// ���\�[�X���J������
	void Release();

	// �e�N�X�`�����\�[�X���擾����
	std::shared_ptr<TextureResource> GetTexture(TextureID id);
	// �t�H���g���\�[�X���擾����
	std::shared_ptr<FontResource> GetFont(FontID id);

private:
	// �e�N�X�`�����\�[�X��ǉ�����
	bool AddTexture(TextureID id, std::shared_ptr<TextureResource>& texture);
	// �t�H���g���\�[�X��ǉ�����
	bool AddFont(FontID id, std::shared_ptr<FontResource>& font);
	template <class T>
	// ���\�[�X��ǉ�����
	void AddResource(std::shared_ptr<T>& resource, int id, std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
		index[id] = resources.size();
		resources.emplace_back(resource);
	}

	template <class T>
	// �L���ȃ��\�[�X���m�F����
	bool CheckIsValid(const std::shared_ptr<T>& resource) {
		return resource->IsValid();
	}

	template <class T>
	// ���\�[�X�����Z�b�g����
	void Reset(std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
		resources.clear();
		resources.shrink_to_fit();
		std::fill(index.begin(), index.end(), NULL_INDEX);
	}

private:
	// �e�N�X�`�����\�[�X
	std::vector<std::shared_ptr<TextureResource>>     m_textures;
	// �e�N�X�`�����\�[�X�̊i�[�ʒu
	std::vector<int>                                  m_texturesIndex;
	// �t�H���g���\�[�X
	std::vector<std::shared_ptr<FontResource>>        m_fonts;
	// �t�H���g���\�[�X�̊i�[�ʒu
	std::vector<int>                                  m_fontsIndex;

};


#endif // !RESOURCE_MANAGER_DEFINED