#include "Resource.h"
#include <Framework\DirectX11.h>
#include "ServiceLocater.h"
#include "ErrorMessage.h"


const std::wstring TextureResource::TEXTURE_DIR = L"Resources/Textures/";
const std::wstring ModelResource::MODEL_DIR     = L"Resources/Models/";
const std::wstring FontResource::FONT_DIR       = L"Resources/Fonts/";

TextureResource::Type Resource<TextureResource::Type, TextureResource::IDType>::m_defaultResource = nullptr;
GeometricPrimitiveResource::Type Resource<GeometricPrimitiveResource::Type, GeometricPrimitiveResource::IDType>::m_defaultResource = nullptr;
ModelResource::Type Resource<ModelResource::Type, ModelResource::IDType>::m_defaultResource = nullptr;
FontResource::Type Resource<FontResource::Type, FontResource::IDType>::m_defaultResource = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="fileName">�ǂݍ��މ摜�̃t�@�C����</param>
TextureResource::TextureResource(const std::wstring& fileName) {
	AddResource(fileName);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TextureResource::~TextureResource() {
}

/// <summary>
/// ���\�[�X��ǉ�����
/// </summary>
/// <param name="fileName">�ǂݍ��މ摜�̃t�@�C����</param>
void TextureResource::AddResource(const std::wstring& fileName) {
	m_resources.emplace_back(nullptr);
	DirectX::CreateWICTextureFromFile(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(TEXTURE_DIR + fileName).c_str(), nullptr, m_resources.back().GetAddressOf());
	if (m_resources.back().Get() == m_defaultResource.Get()) {
		ErrorMessage(L"�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}
	// �摜�T�C�Y�𒲂ׂ�
	ID3D11ShaderResourceView* texture = m_resources.back().Get();
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	texture->GetResource(resource.GetAddressOf());
	D3D11_RESOURCE_DIMENSION dimension;
	resource->GetType(&dimension);
	// Texture2D�łȂ��ꍇ�͏������Ȃ�
	if (dimension != D3D11_RESOURCE_DIMENSION_TEXTURE2D) {
		m_size.emplace_back(DirectX::SimpleMath::Vector2());
		return;
	}
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	resource.As(&texture2D);
	D3D11_TEXTURE2D_DESC desc;
	texture2D->GetDesc(&desc);
	m_size.emplace_back(DirectX::SimpleMath::Vector2(static_cast<float>(desc.Width), static_cast<float>(desc.Height)));
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
/// �摜�T�C�Y���擾����
/// </summary>
/// <param name="index">���\�[�X�̔ԍ�</param>
/// <returns>
/// �摜�T�C�Y
/// </returns>
const DirectX::SimpleMath::Vector2& TextureResource::GetSize(int index) const {
	if (static_cast<int>(m_resources.size()) < index) {
		ErrorMessage(L"�摜�T�C�Y�̎擾�Ŕ͈͊O�ɃA�N�Z�X���܂���");
	}
	return m_size[index];
}

/// <summary>
/// �摜�̒��S���W�ւ̃I�t�Z�b�g���擾����
/// </summary>
/// <param name="index">���\�[�X�̔ԍ�</param>
/// <returns>
/// ���S���W�ւ̃I�t�Z�b�g
/// </returns>
DirectX::SimpleMath::Vector2 TextureResource::GetCenter(int index) const {
	if (static_cast<int>(m_resources.size()) < index) {
		ErrorMessage(L"�摜�̒��S���W�ւ̃I�t�Z�b�g�̎擾�Ŕ͈͊O�ɃA�N�Z�X���܂���");
		return DirectX::SimpleMath::Vector2(0, 0);
	}
	return m_size[index] * 0.5f;
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resource">�W�I���g���b�N�v���~�e�B�u</param>
GeometricPrimitiveResource::GeometricPrimitiveResource(std::unique_ptr<DirectX::GeometricPrimitive>&& resource) {
	m_resources.emplace_back(std::move(resource));
	if (m_resources.back().get() == m_defaultResource.get()) {
		ErrorMessage(L"�W�I���g���b�N�v���~�e�B�u�̐����Ɏ��s���܂���");
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GeometricPrimitiveResource::~GeometricPrimitiveResource() {
}

/// <summary>
/// ���\�[�X���L�����ǂ����m�F����
/// </summary>
/// <param name="index">���\�[�X�̔ԍ�</param>
/// <returns>
/// true  : �L��
/// false : ����
/// </returns>
bool GeometricPrimitiveResource::IsValid(int index) const {
	return m_resources[index].get() != m_defaultResource.get();
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="fileName">�t�@�C����</param>
/// <param name="directory">�f�B���N�g��</param>
ModelResource::ModelResource(const std::wstring& fileName, const std::wstring& directory) {
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	// �G�t�F�N�g�t�@�N�g���[���쐬����
	std::unique_ptr<DirectX::EffectFactory> fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	// �ǂݍ��ނ̃t�@�C���̃f�B���N�g����ݒ肷��
	fxFactory.get()->SetDirectory((MODEL_DIR + directory).c_str());
	// CMO��ǂݍ���Ń��f�����쐬����
	m_resources.emplace_back(DirectX::Model::CreateFromCMO(device, (MODEL_DIR + directory + L"/" + fileName).c_str(), *fxFactory));
	if (m_resources.back().get() == m_defaultResource.get()) {
		ErrorMessage(L"���f���̂ɓǂݍ��ݎ��s���܂���");
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ModelResource::~ModelResource() {

}

/// <summary>
/// ���\�[�X���L�����ǂ����m�F����
/// </summary>
/// <param name="index">���\�[�X�̔ԍ�</param>
/// <returns>
/// true  : �L��
/// false : ����
/// </returns>
bool ModelResource::IsValid(int index) const {
	return m_resources[index].get() != m_defaultResource.get();
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="fileName">�ǂݍ��ރt�H���g�t�@�C��</param>
FontResource::FontResource(const std::wstring& fileName) {
	m_resources.emplace_back(
		std::make_unique<DirectX::SpriteFont>(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(FONT_DIR + fileName).c_str())
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

