#include "EffectManager.h"
#include <Utils\LamdaUtils.h>
#include <Utils\ErrorMessage.h>
#include "EffectFactory.h"
#include "IEffectEmitter.h"


/// <summary>
///	�R���X�g���N�^
/// </summary>
EffectManager::EffectManager()
	: m_effects()
	, m_effectFactory() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EffectManager::~EffectManager() {

}

/// <summary>
/// �G�t�F�N�g�}�l�[�W������������
/// </summary>
void EffectManager::Initialize() {
	m_effectFactory = std::make_unique<EffectFactory>();
	m_effectFactory->Initialize(this);
	m_effects.clear();
	m_effects.resize(m_effectFactory->GetEffectMaxNum(), nullptr);
}

/// <summary>
/// �G�t�F�N�g�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
/// <param name="camera">�J����</param>
void EffectManager::Update(const DX::StepTimer& timer, const Camera* camera) {
	for (std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf()(m_effects, LamdaUtils::NotNull());
		itr != m_effects.end();
		LamdaUtils::FindIfNext(itr, m_effects.end(), LamdaUtils::NotNull())) {
		(*itr)->Update(timer, camera);
		// ���g�p�Ȃ�O��
		if (!(*itr)->IsUsed()) {
			*itr = nullptr;
		}
	}
}

/// <summary>
/// �G�t�F�N�g�}�l�[�W����`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void EffectManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf()(m_effects, LamdaUtils::NotNull());
		itr != m_effects.end();
		LamdaUtils::FindIfNext(itr, m_effects.end(), LamdaUtils::NotNull())) {
		(*itr)->Render(view, proj);
	}
}

/// <summary>
/// �G�t�F�N�g�𐶐�����
/// </summary>
/// <param name="id"></param>
/// <param name="pos"></param>
/// <param name="dir"></param>
IEffectEmitter* EffectManager::CreateEffect(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf()(m_effects, LamdaUtils::IsNull());
	// �󂫂�����ꍇ�ɓo�^����
	if (itr != m_effects.end()) {
		*itr = m_effectFactory->Create(id, pos, dir);
		return *itr;
	}
	
	ErrorMessage(L"�G�t�F�N�g�̋󂫂��Ȃ��A�����ɏo���܂���ł���");
	return nullptr;
}
