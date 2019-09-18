#pragma once
#ifndef UI_OBSERVER_DEFINED
#define UI_OBSERVER_DEFINED


#include "IObserver.h"
#include "UIEvent.h"


/// <summary>
/// UI�Ď��I�u�U�[�o�N���X
/// </summary>
class UIObserver : public IObserver {
public:
	// �R���X�g���N�^
	UIObserver();

public:
	// �I�u�U�[�o���X�V����
	virtual void Update(ISubject* subject);
	// �V�����C�x���g���擾���Ă��邩�m�F
	bool HasNewEvent()const { return m_hasNewEvent; }
	// �C�x���g���擾����
	UIEvent GetEvent();
	// �C�x���gID���擾����
	UIEventID GetEventID();

protected:
	// �V�����C�x���g���擾���Ă��邩�̃t���O
	bool    m_hasNewEvent;
	// �󂯎�����C�x���g
	UIEvent m_reservedEvent;
};

#endif	// !UI_OBSERVER_DEFINED
