#include "UIObserver.h"
#include "ISubject.h"
#include "UISubject.h"
#include "ErrorMessage.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
UIObserver::UIObserver()
	: m_reservedEvent()
	, m_hasNewEvent(false) {
}

/// <summary>
/// �I�u�U�[�o���X�V����
/// </summary>
/// <param name="subject">�T�u�W�F�N�g</param>
void UIObserver::Update(ISubject* subject) {
	// UI�T�u�W�F�N�g���ǂ����m�F
	UISubject* ui_subject = dynamic_cast<UISubject*>(subject);
	if (!ui_subject) {
		ErrorMessage(L"UI�I�u�U�[�o��UI�T�u�W�F�N�g�łȂ����̂���ʒm���󂯂܂���");
		return;
	}
	// �C�x���g���擾
	m_reservedEvent = ui_subject->GetUIEvent();
	// �C�x���g���擾���Ă����Ԃɂ���
	m_hasNewEvent = true;
}

/// <summary>
/// �C�x���g���擾����
/// </summary>
/// <returns>
/// �C�x���g
/// </returns>
UIEvent UIObserver::GetEvent() {
	// �擾�t���O����������
	m_hasNewEvent = false;
	return m_reservedEvent;
}

/// <summary>
/// �C�x���gID���擾����
/// </summary>
/// <returns>
/// �C�x���gID
/// </returns>
UIEventID UIObserver::GetEventID() {
	if (m_hasNewEvent) {
		// �擾�t���O����������
		m_hasNewEvent = false;
		return m_reservedEvent.eventID;
	}
	else {
		// �擾�t���O��false�Ȃ�Null�C�x���g��Ԃ�
		return UIEventID::Null;
	}
}
