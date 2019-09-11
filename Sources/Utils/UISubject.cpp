#include "UISubject.h"
#include "ServiceLocater.h"
#include "MouseWrapper.h"
#include "ErrorMessage.h"
#include "Resource.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="eventID">�C�x���gID</param>
/// <param name="layer">���C���[</param>
/// <param name="pos">���W</param>
/// <param name="size">�T�C�Y</param>
/// <param name="pTexture">�e�N�X�`���ւ̃|�C���^</param>
/// <param name="textureIndex">�e�N�X�`���̃C���f�b�N�X</param>
UISubject::UISubject(UIEventID eventID, int layer, const DirectX::SimpleMath::Vector2& pos,
	const DirectX::SimpleMath::Vector2& size, const TextureResource* pTexture, int textureIndex)
	: m_eventID(eventID)
	, m_layer(layer)
	, m_pos(pos)
	, m_size(size)
	, m_pTexture(pTexture)
	, m_textureIndex(textureIndex)
	, m_text()
	, m_textColor(DirectX::Colors::White)
	, m_pFont() {
}

/// <summary>
/// UI���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void UISubject::Update(const DX::StepTimer& timer) {
	timer;
	MouseWrapper* mouseWrapper = ServiceLocater<MouseWrapper>::Get();
	const DirectX::SimpleMath::Vector2& mouse_pos = mouseWrapper->GetPos();
	// UI�̗̈�����ǂ���
	if (mouse_pos.x - m_pos.x < m_size.x*0.5f &&
		mouse_pos.y - m_pos.y < m_size.y*0.5f) {
		// �N���b�N�������ǂ���
		if (mouseWrapper->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
			OnClick();
		}
		else {
			OnMouseOver();
		}
	}
}

/// <summary>
///	UI��`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UISubject::Render(DirectX::SpriteBatch* spriteBatch) const {
	// �e�N�X�`��������Ε`�悷��
	if (m_pTexture) {
		spriteBatch->Draw(m_pTexture->GetResource(m_textureIndex).Get(),
			m_pos, nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, 1), 0,
			m_pTexture->GetCenter(m_textureIndex), DirectX::SimpleMath::Vector2(1, 1));
	}
	// �e�L�X�g������Ε`�悷��
	if (!m_text.empty()) {
		// �t�H���g�����邩�m�F
		if (!m_pFont) {
			ErrorMessage(L"UI�Ƀt�H���g���ݒ肳��Ă܂���");
			return;
		}
		const DirectX::SpriteFont* font = m_pFont->GetResource().get();
		RECT rect = font->MeasureDrawBounds(m_text.c_str(), DirectX::SimpleMath::Vector2::Zero);
		DirectX::SimpleMath::Vector2 pos(m_pos.x - rect.right*0.5f, m_pos.y - rect.bottom*0.5f);
		font->DrawString(spriteBatch, m_text.c_str(), pos, m_textColor);
	}
}

/// <summary>
/// UI�C�x���g���擾����
/// </summary>
/// <returns>
/// UI�C�x���g
/// </returns>
UIEvent UISubject::GetUIEvent() const {
	UIEvent uiEvent;
	uiEvent.eventID = m_eventID;
	uiEvent.layer = m_layer;
	uiEvent.address = this;
	uiEvent.pos = m_pos;
	return uiEvent;
}

/// <summary>
/// �T�C�Y���e�N�X�`���ɍ��킹��
/// </summary>
void UISubject::FitTextureSize() {
	if (!m_pTexture) {
		ErrorMessage(L"UI�Ƀe�N�X�`�����ݒ肳��Ă��܂���");
		return;
	}
	m_size = m_pTexture->GetSize(m_textureIndex);
}
