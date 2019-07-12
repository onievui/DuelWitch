#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED

#include <numeric>

class IMagic;
enum class PlayerID;


/// <summary>
/// ���@�t�@�N�g���N���X
/// </summary>
class MagicFactory {
public:	
	// ���@��ID
	enum class MagicID {
		Normal,		// �ʏ햂�@
		Fire,       // �t�@�C�A�̖��@
		Thunder,	// �T���_�[�̖��@
		Freeze,     // �t���[�Y�̖��@
	};

private:
	// �e���@�̍ő�o����
	static constexpr int MagicMaxNum[] = {
		30,
		30,
		10,
		10,
	};

	// �z��̃C���f�b�N�X�����ʒu
	static constexpr int MagicBeginIndex[] = {
		0,
		30,
		60,
		70,
	};

public:
	// �R���X�g���N�^
	MagicFactory();
	~MagicFactory();

public:
	// ���@�t�@�N�g��������������
	void Initialize();
	// ���@�𐶐�����
	IMagic* Create(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// �S���@�̍ő�o�������擾����
	static int GetAllMagicMaxNum();
	

private:
	// �Ǘ����閂�@
	std::vector<std::unique_ptr<IMagic>> m_magics;

};


#endif // !MAGIC_FACTORY_DEFINED
