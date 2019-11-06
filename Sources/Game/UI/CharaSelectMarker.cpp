#include "CharaSelectMarker.h"
#include <Utils\ErrorMessage.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="layer">���C���[</param>
/// <param name="pos">���W</param>
/// <param name="pTexture">�e�N�X�`���ւ̃|�C���^</param>
/// <param name="textureIndex">�e�N�X�`���̃C���f�b�N�X</param>
CharaSelectMarker::CharaSelectMarker(int layer, const DirectX::SimpleMath::Vector2& pos, const TextureResource* pTexture, int textureIndex)
	: UISubject(UIEventID::Null, layer, pos, DirectX::SimpleMath::Vector2(100, 100), pTexture, textureIndex)
	, m_enableObserver(true) {

}

/// <summary>
/// �I�u�U�[�o���X�V����
/// </summary>
/// <param name="subject">�T�u�W�F�N�g</param>
void CharaSelectMarker::Update(ISubject* subject) {
	// �I�u�U�[�o�̋@�\�������Ȃ珈�����Ȃ�
	if (!m_enableObserver) {
		return;
	}

	// UI�T�u�W�F�N�g���ǂ����m�F
	UISubject* ui_subject = dynamic_cast<UISubject*>(subject);
	if (!ui_subject) {
		ErrorMessage(L"�L�����Z���N�g�}�[�J�[��UI�T�u�W�F�N�g�łȂ����̂���ʒm���󂯂܂���");
		return;
	}
	// �C�x���g���擾
	m_reservedEvent = ui_subject->GetUIEvent();
	// �C�x���g���擾���Ă����Ԃɂ���
	m_hasNewEvent = true;

	// �}�E�X�I�[�o�[�C�x���g���󂯎�����ꍇ�͍��W���擾����
	if (m_reservedEvent.eventID == UIEventID::MouseOver) {
		m_pos = m_reservedEvent.pos;
	}
}
