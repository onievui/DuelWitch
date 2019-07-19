#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED


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
	static constexpr int MAGIC_NUM[] = {
		30,
		30,
		10,
		10,
	};

	// �z��̃C���f�b�N�X�����ʒu
	static constexpr int MAGIC_BEGIN_INDEX[] = {
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
	static int GetMagicMaxNum();
	
private:
	template <class T>
	// �e���@������������
	void InitializeMagic(MagicID id);

private:
	// �Ǘ����閂�@
	std::vector<std::unique_ptr<IMagic>> m_magics;

};


#endif // !MAGIC_FACTORY_DEFINED


/// <summary>
/// ���@�N���X
/// </summary>
template<class T>
/// <summary>
/// �e���@������������
/// </summary>
/// <param name="id">���@��ID</param>
inline void MagicFactory::InitializeMagic(MagicID id) {
	for (auto itr = m_magics.begin() + MAGIC_BEGIN_INDEX[(int)id], end = itr + MAGIC_NUM[(int)id];
		itr != end; ++itr) {
		*itr = std::make_unique<T>();
	}
}
