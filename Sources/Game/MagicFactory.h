#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED


class IMagic;
class MagicManager;
enum class PlayerID;
enum class MagicID;


/// <summary>
/// ���@�t�@�N�g���N���X
/// </summary>
class MagicFactory {
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
	int GetMagicMaxNum();
	
private:
	template <class T, class... Args>
	// �e���@������������
	void InitializeMagic(MagicID id, Args&&... args);

private:
	// �Ǘ����閂�@
	std::vector<std::unique_ptr<IMagic>> m_magics;
	// ���@�̍ő�o����
	std::vector<int>                     m_maxNum;
	// �z��̃C���f�b�N�X�����ʒu
	std::vector<int>                     m_beginIndex;
};


template<class T, class... Args>
/// <summary>
/// �e���@������������
/// </summary>
/// <param name="id">���@��ID</param>
inline void MagicFactory::InitializeMagic(MagicID id, Args&&... args) {
	for (std::vector<std::unique_ptr<IMagic>>::iterator itr = m_magics.begin() + m_beginIndex[static_cast<int>(id)],
		end = itr + m_maxNum[static_cast<int>(id)]; itr != end; ++itr) {
		*itr = std::make_unique<T>(args...);
	}
}


#endif // !MAGIC_FACTORY_DEFINED

