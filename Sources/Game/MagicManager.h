#pragma once
#ifndef MAGIC_MANAGER_DEFINED
#define MAGIC_MANAGER_DEFINED


#include <Framework\StepTimer.h>
#include "MagicFactory.h"


class IMagic;
class IMagicShooter;


/// <summary>
/// ���@�}�l�[�W���N���X
/// </summary>
class MagicManager {
public:
	// �R���X�g���N�^
	MagicManager();
	// �f�X�g���N�^
	~MagicManager();

public:
	// ���@�}�l�[�W��������������
	void Initialize();
	// ���@�}�l�[�W�����X�V����
	void Update(const DX::StepTimer& timer);
	// ���@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// ���@�𐶐�����
	void CreateMagic(MagicFactory::MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// ���@���擾����
	std::vector<IMagic*>* GetMagics();

private:
	// �����ςݖ��@
	std::vector<IMagic*>                           m_magics;
	// ���@�t�@�N�g��
	std::unique_ptr<MagicFactory>                  m_magicFactory;
	// ���@���˃N���X
	std::vector<std::unique_ptr<IMagicShooter>>    m_magicShooters;

};


#endif // !MAGIC_MANAGER_DEFINED
