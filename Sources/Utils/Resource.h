#pragma once
#ifndef RESOURCE_DEFINED
#define RESOURCE_DEFINED


template <class T>
/// <summary>
/// ���\�[�X�N���X
/// </summary>
class Resource {
public:
	using Type = T;
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
class TextureResource : public Resource<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> {
public:
	// �e�N�X�`�����\�[�X������f�B���N�g����
	static const std::wstring TEXTURE_DIR;

public:
	// �R���X�g���N�^
	TextureResource(const std::wstring& filename);
	// �f�X�g���N�^
	~TextureResource();

public:
	// ���\�[�X���L�����ǂ����m�F����
	bool IsValid(int index = 0) const;
};

/// <summary>
/// �t�H���g���\�[�X
/// </summary>
class FontResource : public Resource<std::unique_ptr<DirectX::SpriteFont>> {
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
