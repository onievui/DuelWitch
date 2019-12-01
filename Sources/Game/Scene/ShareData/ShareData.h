#pragma once
#ifndef SHARD_DATA_DEFINED
#define SHARD_DATA_DEFINED


/// <summary>
/// �V�[���Ԃŋ��L����f�[�^
/// </summary>
class ShareData {
public:
	// �R���X�g���N�^
	ShareData() : m_selectCharaId() {}

public:
	// �I�������L������ID���擾����
	const std::vector<int>& GetSelectCharaID() const             { return m_selectCharaId; }

	// �I�������L������ID��ݒ肷��
	void SetSelectCharaID(const std::vector<int>& selectCharaId) { m_selectCharaId = selectCharaId; }

private:
	// �I�������L������ID
	std::vector<int> m_selectCharaId;

};


#endif // !SHARD_DATA_DEFINED

