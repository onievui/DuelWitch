#pragma once
#ifndef RESOURCE_DEFINED
#define RESOURCE_DEFINED


#include "ResourceID.h"


template <class T, class U>
/// <summary>
/// ���\�[�X�N���X
/// </summary>
class Resource {
public:
	using Type   = T;
	using IDType = U;

public:
	// �R���X�g���N�^
	Resource() {
		m_resources.clear();
	};
	// �f�X�g���N�^
	virtual ~Resource() = default;

public:
	// ���\�[�X���擾����
	const T& GetResource(int index = 0) const {
		return static_cast<int>(m_resources.size()) > index ? m_resources[index] : m_defaultResource;
	}

	// ���\�[�X���L�����ǂ����m�F����
	//bool IsValid(int index = 0);

protected:
	// ���\�[�X
	std::vector<T> m_resources;
	// ���\�[�X�������蓖�Ď��̏��
	static T       m_defaultResource;
};


/// <summary>
/// �e�N�X�`�����\�[�X
/// </summary>
class TextureResource : public Resource<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, TextureID> {
public:
	// �e�N�X�`�����\�[�X������f�B���N�g����
	static const std::wstring TEXTURE_DIR;

public:
	// �R���X�g���N�^
	TextureResource(const std::wstring& fileName);
	// �f�X�g���N�^
	~TextureResource();

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const;
};


/// <summary>
/// �W�I���g���b�N�v���~�e�B�u���\�[�X
/// </summary>
class GeometricPrimitiveResource : public Resource<std::unique_ptr<DirectX::GeometricPrimitive>, GeometricPrimitiveID> {
public:
	// �R���X�g���N�^
	GeometricPrimitiveResource(std::unique_ptr<DirectX::GeometricPrimitive>&& resource);
	// ���[�u�R���X�g���N�^
	GeometricPrimitiveResource(GeometricPrimitiveResource&& from) {
		for (std::vector<std::unique_ptr<DirectX::GeometricPrimitive>>::iterator itr = from.m_resources.begin();
			itr != from.m_resources.end(); ++itr) {
			m_resources.emplace_back(std::move(*itr));
		}
	}
	// �f�X�g���N�^
	~GeometricPrimitiveResource();

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0)const;

};


/// <summary>
/// ���f�������[�X
/// </summary>
class ModelResource :public Resource<std::unique_ptr<DirectX::Model>, ModelID> {
public:
	// ���f�����\�[�X������f�B���N�g����
	static const std::wstring MODEL_DIR;

public:
	// �R���X�g���N�^
	ModelResource(const std::wstring& fileName, const std::wstring& directory = L"");
	// ���[�u�R���X�g���N�^
	ModelResource(ModelResource&& from) {
		for (std::vector<std::unique_ptr<DirectX::Model>>::iterator itr = from.m_resources.begin();
			itr != from.m_resources.end(); ++itr) {
			m_resources.emplace_back(std::move(*itr));
		}
	}
	// �f�X�g���N�^
	~ModelResource();

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0)const;

};


/// <summary>
/// �t�H���g���\�[�X
/// </summary>
class FontResource : public Resource<std::unique_ptr<DirectX::SpriteFont>, FontID> {
public:
	// �t�H���g���\�[�X������f�B���N�g����
	static const std::wstring FONT_DIR;

public:
	// �R���X�g���N�^
	FontResource(const std::wstring& filename);
	// �f�X�g���N�^
	~FontResource();

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const;

};


#endif // !RESOURCE_DEFINED
