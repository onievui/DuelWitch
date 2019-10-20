#pragma once
#ifndef MAGIC_INFO_DEFINED
#define MAGIC_INFO_DEFINED


enum class MagicID;
enum class PlayerID;


/// <summary>
/// ���@�N���X�p���\����
/// </summary>
struct MagicInfo {
	// �R���X�g���N�^
	MagicInfo() {}
	MagicInfo(MagicID _id, PlayerID _playerId, int _level, float _powerRate) 
		: id(_id)
		, playerId(_playerId)
		, level(_level)
		, powerRate(_powerRate)	{
	}

	// ���@ID
	MagicID           id;
	// �v���C���[ID
	PlayerID          playerId;
	// ���@���x��
	int               level;
	// �_���[�W�{��
	float             powerRate;
};

#endif // !MAGIC_INFO_DEFINED