#include "FireMagicShooter.h"
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicFactory.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magics">�����������@�̓o�^��ւ̃|�C���^</param>
/// <param name="magicFactory">���@�t�@�N�g���ւ̃|�C���^</param>
FireMagicShooter::FireMagicShooter(std::vector<IMagic*>* pMagics, MagicFactory* pMagicFactory)
	: m_pMagics(pMagics)
	, m_pMagicFactory(pMagicFactory) {
}

/// <summary>
/// �����@�𔭎˂���
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void FireMagicShooter::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const float angle = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam.wayAngle;
	// 3�����ɕ��򂳂��邽�߂̉�]���쐬����
	DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
	DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
		Math::CreateQuaternionFromVector3(xz_dir, dir));

	//DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::UnitY;
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, -angle);
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
	quaternion.Inverse(quaternion);

	int count = 0;
	// 3�����ɔ��˂���
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_pMagics, LamdaUtils::IsNull());
		itr != m_pMagics->end() && count < 3;
		LamdaUtils::FindIfNext(itr, m_pMagics->end(), LamdaUtils::IsNull()), ++count) {
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
		// ���˂��邽�тɌ��������炷
		direction = DirectX::SimpleMath::Vector3::Transform(direction, quaternion);
	}
	
}
