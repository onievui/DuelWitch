#include "EmptyObject.h"
#include "Collider.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
EmptyObject::EmptyObject() 
	: m_transform()
	, m_collider()
	, m_world(){
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EmptyObject::~EmptyObject() {
	m_collider.reset();
}

/// <summary>
/// �I�u�W�F�N�g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void EmptyObject::Update(const DX::StepTimer& timer) {
	timer;
	m_world = m_transform.GetMatrix();
}

