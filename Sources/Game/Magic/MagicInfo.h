#pragma once
#ifndef MAGIC_INFO_DEFINED
#define MAGIC_INFO_DEFINED


enum class MagicID;
enum class PlayerID;


/// <summary>
/// 魔法クラス用情報構造体
/// </summary>
struct MagicInfo {
	// コンストラクタ
	MagicInfo() {}
	MagicInfo(MagicID _id, PlayerID _playerId, int _level, float _powerRate, int _lockOnPlayerId) 
		: id(_id)
		, playerId(_playerId)
		, level(_level)
		, powerRate(_powerRate)
		, lockOnPlayerId(_lockOnPlayerId) {
	}

	// 魔法ID
	MagicID                 id;
	// プレイヤーID	         
	PlayerID                playerId;
	// 魔法レベル	    	 	    
	int                     level;
	// ダメージ倍率	         
	float                   powerRate;
	// 追尾するプレイヤーのID
	int                     lockOnPlayerId;
};

#endif // !MAGIC_INFO_DEFINED
