#pragma once
#ifndef MAGIC_MANAGER_DEFINED
#define MAGIC_MANAGER_DEFINED


#include <Framework\StepTimer.h>


class IMagic;
class IMagicShooter;
class MagicFactory;
enum class PlayerID;
enum class MagicID;
enum class ElementID;


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
	void CreateMagic(MagicID id, int level, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	void CreateMagic(ElementID id, int level, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// ���@���擾����
	std::vector<IMagic*>* GetMagics();

private:
	// �G�������gID���疂�@ID�ɕϊ�����
	MagicID ElementID2MagicID(ElementID elementId);

private:
	// �����ςݖ��@
	std::vector<IMagic*>                           m_magics;
	// ���@�t�@�N�g��
	std::unique_ptr<MagicFactory>                  m_magicFactory;
	// ���@���˃N���X
	std::vector<std::unique_ptr<IMagicShooter>>    m_magicShooters;

};


#endif // !MAGIC_MANAGER_DEFINED
