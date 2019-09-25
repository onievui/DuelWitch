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

	//���\�[�X��u��������
	void Replace(T&& resource, int index = 0) {
		m_resources[index] = std::forward<T>(resource);
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
	~TextureResource() = default;

public:
	// ���\�[�X��ǉ�����
	void AddResource(const std::wstring& fileName);
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const { return m_resources[index].Get() != m_defaultResource.Get(); }

public:
	// �摜�T�C�Y���擾����
	const DirectX::SimpleMath::Vector2& GetSize(int index = 0) const;
	// �摜�̒��S���W�ւ̃I�t�Z�b�g���擾����
	DirectX::SimpleMath::Vector2 GetCenter(int index = 0) const;

private:
	// �摜�T�C�Y
	std::vector<DirectX::SimpleMath::Vector2> m_size;
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
	~GeometricPrimitiveResource() = default;

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const { return m_resources[index].get() != m_defaultResource.get(); }

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
	~ModelResource() = default;

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const { return m_resources[index].get() != m_defaultResource.get(); }

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
	FontResource(const std::wstring& fileName);
	// ���[�u�R���X�g���N�^
	FontResource(FontResource&& from) {
		for (std::vector<std::unique_ptr<DirectX::SpriteFont>>::iterator itr = from.m_resources.begin();
			itr != from.m_resources.end(); ++itr) {
			m_resources.emplace_back(std::move(*itr));
		}
	}
	// �f�X�g���N�^
	~FontResource() = default;

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const { return m_resources[index].get() != m_defaultResource.get(); }

};


/// <summary>
/// ���_�V�F�[�_���\�[�X
/// </summary>
class VertexShaderResource : public Resource<Microsoft::WRL::ComPtr<ID3D11VertexShader>, VertexShaderID> {
public:
	// ���_�V�F�[�_���\�[�X������f�B���N�g����
	static const std::wstring VERTEX_SHADER_DIR;

public:
	// �R���X�g���N�^
	VertexShaderResource(const std::wstring& fileName, VertexShaderID id);
	// �f�X�g���N�^
	~VertexShaderResource() = default;

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const{ return m_resources[index].Get() != m_defaultResource.Get(); }

public:
	// ���̓��C�A�E�g���擾����
	ID3D11InputLayout* GetInputLayout();

private:
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};


/// <summary>
/// �W�I���g���V�F�[�_���\�[�X
/// </summary>
class GeometryShaderResource : public Resource<Microsoft::WRL::ComPtr<ID3D11GeometryShader>, GeometryShaderID> {
public:
	// ���_�V�F�[�_���\�[�X������f�B���N�g����
	static const std::wstring GEOMETRY_SHADER_DIR;

public:
	// �R���X�g���N�^
	GeometryShaderResource(const std::wstring& fileName);
	// �f�X�g���N�^
	~GeometryShaderResource() = default;

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const { return m_resources[index].Get() != m_defaultResource.Get(); }

};


/// <summary>
/// �s�N�Z���V�F�[�_���\�[�X
/// </summary>
class PixelShaderResource : public Resource<Microsoft::WRL::ComPtr<ID3D11PixelShader>, PixelShaderID> {
public:
	// �s�N�Z���V�F�[�_���\�[�X������f�B���N�g����
	static const std::wstring PIXEL_SHADER_DIR;

public:
	// �R���X�g���N�^
	PixelShaderResource(const std::wstring& fileName);
	// �f�X�g���N�^
	~PixelShaderResource() = default;

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const { return m_resources[index].Get() != m_defaultResource.Get(); }

};


#endif // !RESOURCE_DEFINED
