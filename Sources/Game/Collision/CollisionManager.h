#pragma once
#ifndef COLLISION_MANAGER_DEFINED
#define COLLISION_MANAGER_DEFINED


class Player;
class Field;
class Element;
class IMagic;


/// <summary>
/// �����蔻�菈���N���X
/// </summary>
class CollisionManager {
	// �󂯎��I�u�W�F�N�g�̌^���`���Ă���
	using PlayersData = std::vector<std::unique_ptr<Player>>;
	using FieldData = Field;
	using ElementsData = std::vector<Element*>;
	using MagicsData = std::vector<IMagic*>;

public:
	// �v���C���[�ƃt�B�[���h�̓����蔻����s��
	void CollisionPlayerField(PlayersData* players, FieldData* field);
	// �v���C���[�ƃG�������g�̓����蔻��s��
	void CollisionPlayerElement(PlayersData* players, ElementsData* elements);
	// ���@���m�̓����蔻����s��
	void CollisionMagic(MagicsData* magics);
	// �v���C���[�Ɩ��@�̓����蔻����s��
	void CollisionPlayerMagic(PlayersData* players, MagicsData* magics);
	// �v���C���[���m�̓����蔻����s��
	void CollisionPlayer(PlayersData* players);

};


#endif // !COLLISION_MANAGER_DEFINED
