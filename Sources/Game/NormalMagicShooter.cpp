#include "NormalMagicShooter.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager"></param>
NormalMagicShooter::NormalMagicShooter(MagicManager* magicManager)
	: m_pMagicManager(magicManager) {
}

/// <summary>
/// ���@�̐���
/// </summary>
/// <param name="magicFactory">���@�t�@�N�g���ւ̃|�C���^</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void NormalMagicShooter::Create(MagicFactory* magicFactory, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	for (auto& magic : *m_pMagicManager->GetMagics()) {
		if (!magic) {
			magic = magicFactory->Create(MagicFactory::MagicID::Normal, pos, dir);
			break;
		}
	}
}
