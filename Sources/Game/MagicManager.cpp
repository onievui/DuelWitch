#include "MagicManager.h"
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
	for (std::vector<IMagic*>::iterator itr = m_magics.begin(); itr != m_magics.end(); ++itr) {
		if (*itr) {
			(*itr)->Update(timer);
			if (!(*itr)->IsUsed()) {
				*itr = nullptr;
			}
		}
	}
}


/// <summary>
/// ���@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void MagicManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (std::vector<IMagic*>::const_iterator itr = m_magics.cbegin(); itr != m_magics.cend(); ++itr) {
		if (*itr) {
			(*itr)->Render(view, proj);
		}
	}
}


/// <summary>
/// ���@�𐶐�����
/// </summary>
/// <param name="id">���@ID</param>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vec">����</param>
void MagicManager::CreateMagic(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	m_magicShooters[static_cast<int>(id)]->Create(m_magicFactory.get(), playerId, pos, dir);
}

/// <summary>
/// ���@�𐶐�����
/// </summary>
/// <param name="id">�G�������gID</param>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void MagicManager::CreateMagic(ElementID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	CreateMagic(ElementID2MagicID(id), playerId, pos, dir);
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
