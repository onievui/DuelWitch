#include "FireMagicShooter.h"
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
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

	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, -angle);
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
	quaternion.Inverse(quaternion);

	int count = 0;
	// 3�����ɔ��˂���
	for (IfIterator<std::vector<IMagic*>> itr(*m_pMagics, LamdaUtils::IsNull()); itr != m_pMagics->end() && count < 3; ++itr, ++count) {
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
		// ���˂��邽�тɌ��������炷
		direction = DirectX::SimpleMath::Vector3::Transform(direction, quaternion);
	}

	// �`���[�W���x��1�ȏ�Ȃ�㉺�ɂ����˂���
	DirectX::SimpleMath::Vector3 axis2 = axis.Cross(dir);
	axis2.Normalize();
	if (magicInfo.level >= 1) {
		count = 0;
		for (IfIterator<std::vector<IMagic*>> itr(*m_pMagics, LamdaUtils::IsNull()); itr != m_pMagics->end() && count < 2; ++itr, ++count) {
			quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis2, (count == 0 ? -angle : angle));
			direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
		}
	}

	// �`���[�W���x��2�Ȃ�΂߂ɂ����˂���
	if (magicInfo.level >= 2) {
		count = 0;
		for (IfIterator<std::vector<IMagic*>> itr(*m_pMagics, LamdaUtils::IsNull()); itr != m_pMagics->end() && count < 4; ++itr, ++count) {
			quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, (count % 2 ? -angle/2 : angle/2));
			quaternion *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis2, (count / 2 ? -angle/2 : angle/2));
			direction = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
		}
	}

}
