#include "ScaleUpUI.h"
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
ScaleUpUI::ScaleUpUI(UIEventID eventID, int layer, const DirectX::SimpleMath::Vector2& pos,
	const DirectX::SimpleMath::Vector2& size, const TextureResource* pTexture, int textureIndex)
	: UISubject(eventID, layer, pos, size, pTexture, textureIndex)
	, m_isScaleUp(false)
	, m_clickWaitTime() {
}

/// <summary>
/// UI���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void ScaleUpUI::Update(const DX::StepTimer& timer) {
	UISubject::Update(timer);
	// �^�C�}�[��i�߂�
	if (m_clickWaitTime > 0) {
		float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
		m_clickWaitTime -= elapsed_time;
	}
	// �}�E�X�I�[�o�[���Ɋg�傷��
	m_isScaleUp = m_onMouseOver;
}

/// <summary>
/// UI��`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void ScaleUpUI::Render(DirectX::SpriteBatch* spriteBatch) const {
	// �}�E�X������Ă���Ԃ͊g�傷��
	DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2::One*(m_isScaleUp ? 1.2f : 1.0f);
	// �}�E�X�I�[�o�[���͐ԐF�A�N���b�N���͊D�F�ɂ���
	DirectX::SimpleMath::Color color = DirectX::Colors::White;
	if (m_isScaleUp) {
		if (m_clickWaitTime > 0.0f) {
			color = DirectX::Colors::Gray;
		}
		else {
			color = DirectX::Colors::White;
		}
	}
	color.w *= m_alpha;

	// �e�N�X�`��������Ε`�悷��
	if (m_pTexture) {
		spriteBatch->Draw(m_pTexture->GetResource(m_textureIndex).Get(),
			m_pos, nullptr, color, 0, m_pTexture->GetCenter(m_textureIndex), scale);
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
		DirectX::SimpleMath::Vector2 pos(m_pos.x - rect.right*0.5f*scale.x, m_pos.y - rect.bottom*0.5f*scale.y);
		color = m_textColor;
		color.w *= m_alpha;

		font->DrawString(spriteBatch, m_text.c_str(), pos, color, 0.0f,
			DirectX::SimpleMath::Vector2::Zero, scale);
	}
}

/// <summary>
/// �N���b�N������
/// </summary>
void ScaleUpUI::OnClick() {
	// ���O�ɃN���b�N���Ă��Ȃ���Ώ�������
	if (m_clickWaitTime <= 0.0f) {
		Notify();
		m_clickWaitTime = 0.1f;
	}
}
