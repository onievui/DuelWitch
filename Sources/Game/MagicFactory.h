#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED

class IMagic;

/// <summary>
/// ���@�t�@�N�g���N���X
/// </summary>
class MagicFactory {
public:
	// ���@�̍ő�o����
	static constexpr int MAGIC_MAX_NUM = 30;
	// ���@��ID
	enum class MagicID {
		Normal,		// �ʏ햂�@
		Fire,       // �t�@�C�A�̖��@
		Thunder,	// �T���_�[�̖��@
		Freeze,     // �t���[�Y�̖��@
	};

public:
	// �R���X�g���N�^
	MagicFactory();
	// �f�X�g���N�^
	~MagicFactory();

public:
	// ���@�t�@�N�g��������������
	void Initialize();
	// ���@�𐶐�����
	IMagic* Create(MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);

private:
	// �Ǘ����閂�@
	std::vector<std::unique_ptr<IMagic>> m_magics;

};


#endif // !MAGIC_FACTORY_DEFINED
