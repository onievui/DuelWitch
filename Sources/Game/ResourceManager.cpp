#include "ResourceManager.h"
#include <Utils\ErrorMessage.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
ResourceManager::ResourceManager() {
	Release();
	m_texturesIndex.resize(static_cast<int>(TextureID::Num), NULL_INDEX);
	m_fontsIndex.resize(static_cast<int>(FontID::Num), NULL_INDEX);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ResourceManager::~ResourceManager() {
}

/// <summary>
/// ���\�[�X�����[�h����
/// </summary>
void ResourceManager::Load() {
	AddTexture(TextureID::FireMagicIcon,    std::make_shared<TextureResource>(L"Protected/element1.png"));
	AddTexture(TextureID::FreezeMagicIcon,  std::make_shared<TextureResource>(L"Protected/element2.png"));
	AddTexture(TextureID::ThunderMagicIcon, std::make_shared<TextureResource>(L"Protected/element3.png"));
}

/// <summary>
/// ���\�[�X���J������
/// </summary>
void ResourceManager::Release() {
	Reset(m_textures, m_texturesIndex);
	Reset(m_fonts, m_fontsIndex);
}

/// <summary>
/// �e�N�X�`�����\�[�X���擾����
/// </summary>
/// <param name="id">�e�N�X�`��ID</param>
/// <returns>
/// �e�N�X�`�����\�[�X
/// </returns>
std::shared_ptr<TextureResource> ResourceManager::GetTexture(TextureID id) {
	int index = m_texturesIndex[static_cast<int>(id)];
	if (index == NULL_INDEX) {
		ErrorMessage(L"�摜�̎擾�ŕs���Ȓl���n����܂���");
		return nullptr;
	}
	return m_textures[index];
}

/// <summary>
/// �t�H���g���\�[�X���擾����
/// </summary>
/// <param name="id">�t�H���gID</param>
/// <returns>
/// �t�H���g���\�[�X
/// </returns>
std::shared_ptr<FontResource> ResourceManager::GetFont(FontID id) {
	int index = m_fontsIndex[static_cast<int>(id)];
	if (index == NULL_INDEX) {
		ErrorMessage(L"�t�H���g�̎擾�ŕs���Ȓl���n����܂���");
		return nullptr;
	}
	return m_fonts[index];
}

/// <summary>
/// �e�N�X�`�����\�[�X��ǉ�����
/// </summary>
/// <param name="id">�e�N�X�`��ID</param>
/// <param name="texture">�e�N�X�`�����\�[�X</param>
/// <returns>
/// true  : �ǉ��ɐ���
/// false : �ǉ��Ɏ��s
/// </returns>
bool ResourceManager::AddTexture(TextureID id, std::shared_ptr<TextureResource>& texture) {
	if (!CheckIsValid(texture)) {
		ErrorMessage(L"�摜�̓ǂݍ��݂Ɏ��s���܂���");
		return false;
	}
	AddResource(texture, static_cast<int>(id), m_textures, m_texturesIndex);
	return true;
}

/// <summary>
/// �t�H���g���\�[�X��ǉ�����
/// </summary>
/// <param name="id">�t�H���gID</param>
/// <param name="font">�t�H���g���\�[�X</param>
/// <returns>
/// true  : �ǉ��ɐ���
/// false : �ǉ��Ɏ��s
/// </returns>
bool ResourceManager::AddFont(FontID id, std::shared_ptr<FontResource>& font) {
	if (!CheckIsValid(font)) {
		ErrorMessage(L"�t�H���g�̓ǂݍ��݂Ɏ��s���܂���");
		return false;
	}
	AddResource(font, static_cast<int>(id), m_fonts, m_fontsIndex);
	return true;
}

