#include "ThunderMagicShooter.h"
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Utils\MathUtils.h>
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
	if (magicInfo.level == 0) {
		std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_pMagics, LamdaUtils::IsNull());
		if (itr != m_pMagics->end()) {
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, dir);
		}
	}
	else if (magicInfo.level >= 1) {
		// TODO: ��2way�p�x�萔�ɂ���
		const float angle = Math::PI / 20;
		int count = 0;
		// 2�����ɕ��򂳂��邽�߂̉�]���쐬����
		DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
		DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
			Math::CreateQuaternionFromVector3(xz_dir, dir));

		for (IfIterator<std::vector<IMagic*>> itr(*m_pMagics, LamdaUtils::IsNull()); itr != m_pMagics->end() && count < 2; ++itr, ++count) {
			DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, (count == 0 ? -angle : angle));
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
		}
	}
	
}
