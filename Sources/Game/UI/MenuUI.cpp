#include "MenuUI.h"
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\AudioManager.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="eventID">�C�x���gID</param>
/// <param name="layer">���C���[</param>
/// <param name="pos">���W</param>
/// <param name="size">�T�C�Y</param>
/// <param name="pTexture">�e�N�X�`���ւ̃|�C���^</param>
/// <param name="textureIndex">�e�N�X�`���̃C���f�b�N�X</param>
MenuUI::MenuUI(UIEventID eventID, int layer,
	const DirectX::SimpleMath::Vector2& pos, const DirectX::SimpleMath::Vector2& size,
	const TextureResource* pTexture, int textureIndex)
	: ScaleUpUI(eventID, layer, pos, size, pTexture, textureIndex)
	, m_isSelected(false)
	, m_onMouseEnterSoundId(SoundID::Cursor)
	, m_onClickSoundId(SoundID::Decision) {
}

/// <summary>
/// UI���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void MenuUI::Update(const DX::StepTimer& timer) {
	// �p�b�h���ڑ��̏ꍇ
	if (!ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		UISubject::Update(timer);
		// �}�E�X�I�[�o�[���Ɋg�傷��
		m_isScaleUp = m_onMouseOver;
	}
	// �p�b�h�ڑ��̏ꍇ
	else {
		m_isScaleUp = m_isSelected;
		// �{�^�����������Ƃ��ɃN���b�N��������
		const bool button_pressed = ServiceLocater<InputManager>::Get()->IsPressed(InputID::Desicion);
		if (m_isSelected && button_pressed) {
			OnClick();
		}
	}

	// �^�C�}�[��i�߂�
	if (m_clickWaitTime > 0) {
		float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
		m_clickWaitTime -= elapsed_time;
	}
	
}

/// <summary>
/// �}�E�X�N��������
/// </summary>
void MenuUI::OnMouseEnter() {
	// ���ʉ���炷
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onMouseEnterSoundId);
	m_textureIndex = 1;
}

/// <summary>
/// �}�E�X���E������
/// </summary>
void MenuUI::OnMouseExit() {
	m_textureIndex = 0;
}

/// <summary>
/// �N���b�N������
/// </summary>
void MenuUI::OnClick() {
	ScaleUpUI::OnClick();
	// ���ʉ���炷������ǉ�����
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onClickSoundId);
}

/// <summary>
/// UI��I������
/// </summary>
void MenuUI::Select() {
	m_isSelected = true;
	// �p�b�h�ڑ����͑I�������Ƃ��Ƀ}�E�X�N����������
	if (ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		OnMouseEnter();
	}
}
