#pragma once
#ifndef PLAYER_DATA_DEFINED
#define PLAYER_DATA_DEFINED


class Transform;


/// <summary>
/// �v���C���[���\����
/// </summary>
struct PlayerData {
	std::vector<const Transform*> transforms;
};


#endif // !PLAYER_DATA_DEFINED
