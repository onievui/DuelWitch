#include "Resource.h"
#include <Framework\DirectX11.h>
#include "ServiceLocater.h"
#include "ErrorMessage.h"


const std::wstring TextureResource::TEXTURE_DIR = L"Resources/Textures/";
const std::wstring FontResource::FONT_DIR       = L"Resources/Textures/";

TextureResource::Type Resource<TextureResource::Type>::m_defaultResource = nullptr;
FontResource::Type Resource<FontResource::Type>::m_defaultResource = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="filename">�ǂݍ��މ摜�̃t�@�C����</param>
TextureResource::TextureResource(const std::wstring& filename) {
	m_resources.emplace_back(nullptr);
	DirectX::CreateWICTextureFromFile(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(TEXTURE_DIR + filename).c_str(), nullptr, m_resources.back().GetAddressOf());
	if (m_resources.back().Get() == m_defaultResource.Get()) {
		ErrorMessage(L"�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TextureResource::~TextureResource() {
}

/// <summary>
/// ���\�[�X���L�����ǂ����m�F����
/// </summary>
/// <param name="index">���\�[�X�̔ԍ�</param>
/// <returns>
/// true  : �L��
/// false : ����
/// </returns>
bool TextureResource::IsValid(int index) const {
	return m_resources[index].Get() != m_defaultResource.Get();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="filename">�ǂݍ��ރt�H���g�t�@�C��</param>
FontResource::FontResource(const std::wstring& filename) {
	m_resources.emplace_back(
		std::make_unique<DirectX::SpriteFont>(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(FONT_DIR + filename).c_str())
	);
	if (m_resources.back().get() == m_defaultResource.get()) {
		ErrorMessage(L"�t�H���g�̓ǂݍ��݂Ɏ��s���܂���");
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FontResource::~FontResource() {
}

/// <summary>
/// ���\�[�X���L�����ǂ����m�F����
/// </summary>
/// <param name="index">���\�[�X�̔ԍ�</param>
/// <returns>
/// true  : �L��
/// false : ����
/// </returns>
bool FontResource::IsValid(int index) const {
	return m_resources[index].get() != m_defaultResource.get();
}

