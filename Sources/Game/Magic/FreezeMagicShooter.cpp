#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include "FreezeMagicShooter.h"
#include "MagicFactory.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magics">�����������@�̓o�^��ւ̃|�C���^</param>
/// <param name="magicFactory">���@�t�@�N�g���ւ̃|�C���^</param>
FreezeMagicShooter::FreezeMagicShooter(std::vector<IMagic*>* pMagics, MagicFactory* pMagicFactory)
	: m_pMagics(pMagics)
	, m_pMagicFactory(pMagicFactory) {
}

/// <summary>
/// �X���@�𔭎˂���
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void FreezeMagicShooter::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	IfIterator<std::vector<IMagic*>> itr(*m_pMagics, LamdaUtils::IsNull());
	if (itr != m_pMagics->end()) {
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, dir);
	}
	
	// �`���[�W���x��1�Ȃ�2�o���i�n�C�X�s�[�h�j
	if (magicInfo.level >= 1) {
		++itr;
		if (itr != m_pMagics->end()) {
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, -dir);
		}
	}
}
