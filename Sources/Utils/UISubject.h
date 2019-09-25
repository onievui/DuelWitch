#pragma once
#ifndef UI_SUBJECT_DEFINED
#define UI_SUBJECT_DEFINED


#include <Framework\StepTimer.h>
#include "Subject.h"
#include "UIEvent.h"


class TextureResource;
class FontResource;


/// <summary>
/// UI�T�u�W�F�N�g�N���X
/// </summary>
class UISubject : public Subject {
public:
	// �R���X�g���N�^
	UISubject(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);

public:
	// UI���X�V����
	virtual void Update(const DX::StepTimer& timer);
	// UI��`�悷��
	virtual void Render(DirectX::SpriteBatch* spriteBatch) const;

public:
	// UI�C�x���g���擾����
	UIEvent GetUIEvent() const;
	// ���W���擾����
	const DirectX::SimpleMath::Vector2& GetPos() const             { return m_pos; }
	// ���W��ݒ肷��
	void SetPos(const DirectX::SimpleMath::Vector2& pos)           { m_pos = pos; }
	// �T�C�Y���擾����
	const DirectX::SimpleMath::Vector2& GetSize() const            { return m_size; }
	// �T�C�Y��ݒ肷��
	void SetSize(const DirectX::SimpleMath::Vector2& size)         { m_size = size; }
	// �X�P�[�����擾����
	const DirectX::SimpleMath::Vector2& GetScale() const           { return m_scale; }
	// �X�P�[����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector2& scale);
	// ���C���[���擾����
	int GetLayer() const                                           { return m_layer; }
	// ���C���[��ݒ肷��
	void SetLayer(int layer)                                       { m_layer = layer; }
	// �e�N�X�`�����擾����
	const TextureResource* GetTexture() const                      { return m_pTexture; }
	// �e�N�X�`����ݒ肷��
	void SetTexture(const TextureResource* pTexture)               { m_pTexture = pTexture; }
	// �e�N�X�`���̃C���f�b�N�X���擾����
	int GetTextureIndex() const                                    { return m_textureIndex; }
	// �e�N�X�`���̃C���f�b�N�X��ݒ肷��
	void SetTextureIndex(int textureIndex)                         { m_textureIndex = textureIndex; }
	// �e�L�X�g���擾����
	const std::wstring& GetText() const                            { return m_text; }
	// �e�L�X�g��ݒ肷��
	void SetText(const std::wstring& text)                         { m_text = text; }
	// �e�L�X�g�F���擾����
	const DirectX::SimpleMath::Color& GetTextColor() const         { return m_textColor; }
	// �e�L�X�g�F��ݒ肷��
	void SetTextColor(const DirectX::SimpleMath::Color& textColor) { m_textColor = textColor; }
	// �t�H���g���擾����
	const FontResource* GetFont()const                             { return m_pFont; }
	// �t�H���g��ݒ肷��
	void SetFont(const FontResource* pFont)                        { m_pFont = pFont; }

	// �T�C�Y���e�N�X�`���ɍ��킹��
	void FitTextureSize();

protected:
	// �N���b�N������
	virtual void OnClick();
	// �}�E�X�I�[�o�[������
	virtual void OnMouseOver() {}
	// �ҋ@������
	virtual void OnIdle() {}

protected:
	// UI�C�x���gID
	UIEventID                    m_eventID;
	// ���C���[
	int                          m_layer;
	// ���S���W
	DirectX::SimpleMath::Vector2 m_pos;
	// �T�C�Y
	DirectX::SimpleMath::Vector2 m_size;
	// �X�P�[��
	DirectX::SimpleMath::Vector2 m_scale;
	// �e�N�X�`��
	const TextureResource*       m_pTexture;
	// �e�N�X�`���̃C���f�b�N�X
	int                          m_textureIndex;
	// �e�L�X�g
	std::wstring                 m_text;
	// �e�L�X�g�F
	DirectX::SimpleMath::Color   m_textColor;
	// �t�H���g
	const FontResource*          m_pFont;
};

#endif // !UI_SUBJECT_DEFINED