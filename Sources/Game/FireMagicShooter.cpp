#include "FireMagicShooter.h"
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "IMagic.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager">���@�}�l�[�W���ւ̃|�C���^</param>
FireMagicShooter::FireMagicShooter(MagicManager* pMagicManager)
	: m_pMagicManager(pMagicManager) {
}

/// <summary>
/// �����@�𔭎˂���
/// </summary>
/// <param name="magicFactory">���@�t�@�N�g���ւ̃|�C���^</param>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void FireMagicShooter::Create(MagicFactory* magicFactory, const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& dir) {
	const float angle = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam.wayAngle;
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::UnitY, -angle);
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
	quaternion.Inverse(quaternion);

	std::vector<IMagic*>* magics = m_pMagicManager->GetMagics();
	int count = 0;
	// 3�����ɔ��˂���
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*magics, LamdaUtils::IsNull());
		itr != magics->end() && count < 3;
		LamdaUtils::FindIfNext(itr, magics->end(), LamdaUtils::IsNull()), ++count) {
		(*itr) = magicFactory->Create(magicInfo, pos, direction);
		// ���˂��邽�тɌ��������炷
		direction = DirectX::SimpleMath::Vector3::Transform(direction, quaternion);
	}
	
}
