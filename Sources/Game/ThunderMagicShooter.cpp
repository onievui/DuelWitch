#include "ThunderMagicShooter.h"
#include <Utils\LamdaUtils.h>
#include "MagicFactory.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magics">�����������@�̓o�^��ւ̃|�C���^</param>
/// <param name="magicFactory">���@�t�@�N�g���ւ̃|�C���^</param>
ThunderMagicShooter::ThunderMagicShooter(std::vector<IMagic*>* pMagics, MagicFactory* pMagicFactory)
	: m_pMagics(pMagics)
	, m_pMagicFactory(pMagicFactory) {
}

/// <summary>
/// �����@�𔭎˂���
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void ThunderMagicShooter::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// �����ɔ��˂���
	DirectX::SimpleMath::Vector3 direction = dir;
	direction.y = 0.0f;

	std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_pMagics, LamdaUtils::IsNull());
	if (itr != m_pMagics->end()) {
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, dir);
	}
	
}
