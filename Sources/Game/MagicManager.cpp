#include "MagicManager.h"
#include <Utils\LamdaUtils.h>
#include "MagicFactory.h"
#include "MagicID.h"
#include "ElementFactory.h"
#include "IMagic.h"
#include "IMagicShooter.h"
#include "NormalMagicShooter.h"
#include "FireMagicShooter.h"
#include "ThunderMagicShooter.h"
#include "ThunderStrikeMagicShooter.h"
#include "FreezeMagicShooter.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
MagicManager::MagicManager() 
	: m_magics()
	, m_magicFactory()
	, m_magicShooters() {

}


/// <summary>
/// �f�X�g���N�^
/// </summary>
MagicManager::~MagicManager() {
}


/// <summary>
/// ���@�}�l�[�W��������������
/// </summary>
void MagicManager::Initialize() {
	m_magicFactory = std::make_unique<MagicFactory>();
	m_magicFactory->Initialize(this);
	m_magics.clear();
	m_magics.resize(m_magicFactory->GetMagicMaxNum(), nullptr);
	m_magicShooters.resize(static_cast<int>(MagicID::Num));
	m_magicShooters[static_cast<int>(MagicID::Normal)]        = std::make_unique<NormalMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::Fire)]          = std::make_unique<FireMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::Thunder)]       = std::make_unique<ThunderMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::ThunderStrike)] = std::make_unique<ThunderStrikeMagicShooter>(this);
	m_magicShooters[static_cast<int>(MagicID::Freeze)]        = std::make_unique<FreezeMagicShooter>(this);
}


/// <summary>
/// ���@�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void MagicManager::Update(const DX::StepTimer& timer) {
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(m_magics, LamdaUtils::NotNull());
		itr != m_magics.end();
		LamdaUtils::FindIfNext(itr, m_magics.end(), LamdaUtils::NotNull())) {
		(*itr)->Update(timer);
		// �g���Ȃ��Ȃ�����I���������s��
		if (!(*itr)->IsUsed()) {
			(*itr)->Finalize();
			*itr = nullptr;
		}
	}
}


/// <summary>
/// ���@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void MagicManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(m_magics, LamdaUtils::NotNull());
		itr != m_magics.end();
		LamdaUtils::FindIfNext(itr, m_magics.end(), LamdaUtils::NotNull())) {
		(*itr)->Render(view, proj);
	}
}


/// <summary>
/// ���@�𐶐�����
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="vec">����</param>
void MagicManager::CreateMagic(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	m_magicShooters[static_cast<int>(magicInfo.id)]->Create(m_magicFactory.get(), magicInfo, pos, dir);
}

/// <summary>
/// ���@�𐶐�����
/// </summary>
/// <param name="id">�G�������gID</param>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void MagicManager::CreateMagic(ElementID id, const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	MagicInfo info = magicInfo;
	info.id = ElementID2MagicID(id);
	CreateMagic(magicInfo, pos, dir);
}


/// <summary>
/// ���@���擾����
/// </summary>
/// <returns>
/// ���@�̔z��
/// </returns>
std::vector<IMagic*>* MagicManager::GetMagics() {
	return &m_magics;
}

/// <summary>
/// �G�������gID���疂�@ID�ɕϊ�����
/// </summary>
/// <param name="elementId">�G�������gID</param>
/// <returns>
/// ���@ID
/// </returns>
MagicID MagicManager::ElementID2MagicID(ElementID elementId) {
	switch (elementId) {
	case ElementID::Fire:
		return MagicID::Fire;
	case ElementID::Thunder:
		return MagicID::Thunder;
	case ElementID::Freeze:
		return MagicID::Freeze;
	default:
		return MagicID::Normal;
	}
}
