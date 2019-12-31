#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Utils\MathUtils.h>
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
		// XZ�����ɕϊ�����
		DirectX::SimpleMath::Vector3 direction(dir.x, 0, dir.z);
		direction.Normalize();
		(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
	}
	else {
		return;
	}
	
	// �`���[�W���x��1�ȏ�Ȃ�2�o���i�n�C�X�s�[�h�j
	if (magicInfo.level >= 1) {
		++itr;
		if (itr != m_pMagics->end()) {
			// XZ�����ɕϊ�����
			DirectX::SimpleMath::Vector3 direction(dir.x, 0, dir.z);
			direction.Normalize();
			(*itr) = m_pMagicFactory->Create(magicInfo, pos, -direction);
		}
		else {
			return;
		}
	}

	// �`���[�W���x��2�Ȃ炳���2�o���i�n�C�X�s�[�h�j
	if (magicInfo.level >= 2) {

		DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
			Math::CreateQuaternionFromVector3(xz_dir, dir));

		direction = DirectX::SimpleMath::Vector3::UnitY;

		for (int i = 0; i < 2; ++i) {
			++itr;
			if (itr != m_pMagics->end()) {
				(*itr) = m_pMagicFactory->Create(magicInfo, pos, direction);
			}
			else {
				return;
			}
			direction *= -1;
		}
	}
}
