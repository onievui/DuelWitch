#include "MagicManager.h"
#include "IMagic.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
MagicManager::MagicManager() 
	: m_magics()
	, m_magicFactory() {

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
	m_magics.resize(MagicFactory::MAGIC_MAX_NUM, nullptr);
	m_magicFactory = std::make_unique<MagicFactory>();
	m_magicFactory->Initialize();
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
/// <param name="id">���@��ID</param>
/// <param name="pos">���W</param>
/// <param name="vec">����</param>
void MagicManager::CreateMagic(MagicFactory::MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vec) {
	IMagic* created_magic = m_magicFactory->Create(id, pos, vec);
	for (auto& magic : m_magics) {
		if (!magic) {
			magic = created_magic;
			break;
		}
	}
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
