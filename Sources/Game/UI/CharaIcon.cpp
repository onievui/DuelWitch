#include "CharaIcon.h"
#include <Utils\Resource.h>
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\AudioManager.h>



/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="layer">���C���[</param>
/// <param name="pos">���W</param>
/// <param name="pTexture">�e�N�X�`���ւ̃|�C���^</param>
/// <param name="textureIndex">�e�N�X�`���̃C���f�b�N�X</param>
CharaIcon::CharaIcon(int layer, const DirectX::SimpleMath::Vector2& pos, const TextureResource* pTexture, int textureIndex)
	: UISubject(UIEventID::CharaIcon, layer, pos, DirectX::SimpleMath::Vector2(100, 100), pTexture, textureIndex)
	, m_isSelected(false) {
	// �e�N�X�`���̃T�C�Y��ǂݍ���
	if (pTexture) {
		m_size = pTexture->GetSize();
	}
}

/// <summary>
/// UI���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CharaIcon::Update(const DX::StepTimer& timer) {
	// �p�b�h���ڑ��̏ꍇ
	if (!ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		UISubject::Update(timer);
	}
	// �p�b�h�ڑ��őI�𒆂̏ꍇ
	else if (m_isSelected) {
		// �{�^�����������Ƃ��ɃN���b�N��������
		const bool button_pressed = ServiceLocater<InputManager>::Get()->IsPressed(InputID::Desicion);
		if (button_pressed) {
			OnClick();
		}
		// ����ȊO�̓}�E�X�I�[�o�[���Ă���ƈ���
		else {
			OnMouseOver();
		}
	}

}

/// <summary>
/// �}�E�X�N��������
/// </summary>
void CharaIcon::OnMouseEnter() {
	// ���ʉ���炷
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Cursor);
}

/// <summary>
/// �}�E�X�I�[�o�[������
/// </summary>
void CharaIcon::OnMouseOver() {
	// �ꎞ�I�ɃC�x���gID���}�E�X�I�[�o�[�ɕς��Ă���ʒm����
	UIEventID tmpID = m_eventID;
	m_eventID = UIEventID::MouseOver;
	Notify();
	m_eventID = tmpID;
}

/// <summary>
/// �N���b�N������
/// </summary>
void CharaIcon::OnClick() {
	UISubject::OnClick();
	// ���ʉ���炷
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Decision);
}

/// <summary>
/// UI��I������
/// </summary>
void CharaIcon::Select() {
	m_isSelected = true;
	// �p�b�h�ڑ����͑I�������Ƃ��Ƀ}�E�X�N����������
	if (ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		OnMouseEnter();
	}
}
