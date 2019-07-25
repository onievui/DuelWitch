#include "MagicManager.h"
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
	m_magics.clear();
	m_magics.resize(MagicFactory::GetMagicMaxNum(), nullptr);
	m_magicFactory = std::make_unique<MagicFactory>();
	m_magicFactory->Initialize(this);
	m_magicShooters.resize(5);
	m_magicShooters[(int)MagicFactory::MagicID::Normal]        = std::make_unique<NormalMagicShooter>(this);
	m_magicShooters[(int)MagicFactory::MagicID::Fire]          = std::make_unique<FireMagicShooter>(this);
	m_magicShooters[(int)MagicFactory::MagicID::Thunder]       = std::make_unique<ThunderMagicShooter>(this);
	m_magicShooters[(int)MagicFactory::MagicID::ThunderStrike] = std::make_unique<ThunderStrikeMagicShooter>(this);
	m_magicShooters[(int)MagicFactory::MagicID::Freeze]        = std::make_unique<FreezeMagicShooter>(this);
}


/// <summary>
/// ���@�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void MagicManager::Update(const DX::StepTimer& timer) {
	for (auto& magic : m_magics) {
		if (magic) {
			magic->Update(timer);
			if (!magic->IsUsed()) {
				magic = nullptr;
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
	for (const auto& magic : m_magics) {
		if (magic) {
			magic->Render(view, proj);
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
void MagicManager::CreateMagic(MagicFactory::MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	m_magicShooters[(int)id]->Create(m_magicFactory.get(), playerId, pos, dir);
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
MagicFactory::MagicID MagicManager::ElementID2MagicID(ElementID elementId) {
	switch (elementId) {
	case ElementID::Fire:
		return MagicFactory::MagicID::Fire;
	case ElementID::Thunder:
		return MagicFactory::MagicID::Thunder;
	case ElementID::Freeze:
		return MagicFactory::MagicID::Freeze;
	default:
		return MagicFactory::MagicID::Normal;
	}
}