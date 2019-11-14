#include "CollisionManager.h"
#include <Utils\LamdaUtils.h>
#include <Game\Collision\Collision.h>
#include <Game\Player\Player.h>
#include <Game\Field\Field.h>
#include <Game\Element\Element.h>
#include <Game\Magic\IMagic.h>


/// <summary>
/// プレイヤーとフィールドの当たり判定を行う
/// </summary>
/// <param name="players">プレイヤー</param>
/// <param name="field">フィールド</param>
void CollisionManager::CollisionPlayerField(PlayersData* players, FieldData* field) {
	for (PlayersData::iterator itr = players->begin(); itr != players->end(); ++itr) {
		field->CollisionCheckPlayer(itr->get());
	}
}

/// <summary>
/// プレイヤーとエレメントの当たり判定を行う
/// </summary>
/// <param name="players">プレイヤー</param>
/// <param name="elements">エレメント</param>
void CollisionManager::CollisionPlayerElement(PlayersData* players, ElementsData* elements) {
	// 未使用のオブジェクトは飛ばすようにする
	auto live_check = LamdaUtils::NotNull();

	for (ElementsData::iterator element_itr = LamdaUtils::FindIf(*elements, live_check),
		element_end = elements->end();
		element_itr != element_end;
		LamdaUtils::FindIfNext(element_itr, element_end, live_check)) {
		const Collider* element_collider = (*element_itr)->GetCollider();
		for (PlayersData::iterator player_itr = players->begin(); player_itr != players->end(); ++player_itr) {
			if (Collision::HitCheck(element_collider, (*player_itr)->GetCollider())) {
				// プレイヤーにエレメントを取得させる
				(*player_itr)->GetElement((*element_itr)->GetID());
				// エレメントを未使用にする
				(*element_itr)->SetUsed(false);
				break;
			}
		}
	}
}

/// <summary>
/// 魔法同士の当たり判定を行う
/// </summary>
/// <param name="magics">魔法</param>
void CollisionManager::CollisionMagic(MagicsData* magics) {
	// 未使用のオブジェクトは飛ばすようにする
	auto live_check = LamdaUtils::NotNull();

	for (MagicsData::iterator itr = LamdaUtils::FindIf(*magics, live_check),
		end = magics->end();
		itr != end;) {
		const Collider* collider = (*itr)->GetCollider();
		MagicsData::iterator next = std::find_if(itr + 1, end, live_check);
		for (MagicsData::iterator itr2 = next; itr2 != end; itr2 = std::find_if(itr2 + 1, end, live_check)) {
			// 同一プレイヤーの魔法なら判定しない
			if ((*itr)->GetPlayerID() == (*itr2)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(collider, (*itr2)->GetCollider())) {
				// 衝突したことを通知する
				(*itr)->HitMagic(*itr2);
				(*itr2)->HitMagic(*itr);
			}
		}
		itr = next;
	}
}

/// <summary>
/// プレイヤーと魔法の当たり判定を行う
/// </summary>
/// <param name="players">プレイヤー</param>
/// <param name="magics">魔法</param>
void CollisionManager::CollisionPlayerMagic(PlayersData* players, MagicsData* magics) {
	// 未使用のオブジェクトは飛ばすようにする
	auto live_check = LamdaUtils::NotNull();

	for (MagicsData::iterator magic_itr = LamdaUtils::FindIf(*magics, live_check),
		magic_end = magics->end();
		magic_itr != magic_end;
		LamdaUtils::FindIfNext(magic_itr, magic_end, live_check)) {
		const Collider* magic_collider = (*magic_itr)->GetCollider();
		for (PlayersData::iterator player_itr = players->begin(); player_itr != players->end(); ++player_itr) {
			// 自身の魔法とは判定しない
			if ((*player_itr)->GetPlayerID() == (*magic_itr)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(magic_collider, (*player_itr)->GetCollider())) {
				// 魔法にプレイヤーが衝突したことを通知する
				(*magic_itr)->HitPlayer((*player_itr)->GetCollider());
				// プレイヤーに魔法が衝突したことを通知する
				(*player_itr)->HitMagic(*magic_itr);
			}
		}
	}
}

/// <summary>
/// プレイヤー同士の当たり判定を行う
/// </summary>
/// <param name="players">プレイヤー</param>
void CollisionManager::CollisionPlayer(PlayersData* players) {
	for (PlayersData::iterator itr1 = players->begin(); itr1 != players->end() - 1; ++itr1) {
		const Collider* collider1 = (*itr1)->GetCollider();
		for (PlayersData::iterator itr2 = itr1 + 1; itr2 != players->end(); ++itr2) {
			if (Collision::HitCheck(collider1, (*itr2)->GetCollider())) {
				// 衝突したことを通知する
				(*itr1)->HitPlayer(**itr2);
				(*itr2)->HitPlayer(**itr1);
			}
		}
	}
}
