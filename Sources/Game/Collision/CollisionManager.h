#pragma once
#ifndef COLLISION_MANAGER_DEFINED
#define COLLISION_MANAGER_DEFINED


class Player;
class Field;
class Element;
class IMagic;


/// <summary>
/// 当たり判定処理クラス
/// </summary>
class CollisionManager {
	// 受け取るオブジェクトの型を定義しておく
	using PlayersData = std::vector<std::unique_ptr<Player>>;
	using FieldData = Field;
	using ElementsData = std::vector<Element*>;
	using MagicsData = std::vector<IMagic*>;

public:
	// プレイヤーとフィールドの当たり判定を行う
	void CollisionPlayerField(PlayersData* players, FieldData* field);
	// プレイヤーとエレメントの当たり判定行う
	void CollisionPlayerElement(PlayersData* players, ElementsData* elements);
	// 魔法同士の当たり判定を行う
	void CollisionMagic(MagicsData* magics);
	// プレイヤーと魔法の当たり判定を行う
	void CollisionPlayerMagic(PlayersData* players, MagicsData* magics);
	// プレイヤー同士の当たり判定を行う
	void CollisionPlayer(PlayersData* players);

};


#endif // !COLLISION_MANAGER_DEFINED
