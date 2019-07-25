#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED


class IMagic;
class MagicManager;
enum class PlayerID;


/// <summary>
/// ���@�t�@�N�g���N���X
/// </summary>
class MagicFactory {
public:	
	// ���@��ID
	enum class MagicID {
		Normal,		    // �ʏ햂�@
		Fire,           // �����@
		Thunder,	    // �����@
		ThunderStrike,	// �������@
		Freeze,         // �X���@
	};

private:
	// �e���@�̍ő�o����
	static constexpr int MAGIC_NUM[] = {
		30,
		30,
		10,
		10,
		10,
	};

	// �z��̃C���f�b�N�X�����ʒu
	static constexpr int MAGIC_BEGIN_INDEX[] = {
		0,
		30,
		60,
		70,
		80,
	};

public:
	// �R���X�g���N�^
	MagicFactory();
	~MagicFactory();

public:
	// ���@�t�@�N�g��������������
	void Initialize(MagicManager* magicManager);
	// ���@�𐶐�����
	IMagic* Create(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// �S���@�̍ő�o�������擾����
	static int GetMagicMaxNum();
	
private:
	template <class T, class... Args>
	// �e���@������������
	void InitializeMagic(MagicID id, Args&&... args);

private:
	// �Ǘ����閂�@
	std::vector<std::unique_ptr<IMagic>> m_magics;

};


template<class T, class... Args>
/// <summary>
/// �e���@������������
/// </summary>
/// <param name="id">���@��ID</param>
inline void MagicFactory::InitializeMagic(MagicID id, Args&&... args) {
	for (auto itr = m_magics.begin() + MAGIC_BEGIN_INDEX[(int)id], end = itr + MAGIC_NUM[(int)id];
		itr != end; ++itr) {
		*itr = std::make_unique<T>(args...);
	}
}


#endif // !MAGIC_FACTORY_DEFINED
