#include "CollisionManager.h"
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
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
		// フィールドクラス内で内外判定を行う
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

	for (IfIterator<ElementsData> element_itr(*elements, live_check); element_itr != elements->end(); ++element_itr) {
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

	for (IfIterator<MagicsData> itr(*magics, live_check);itr != magics->end();) {
		const Collider* collider = (*itr)->GetCollider();
		// 次のループのカウンターの処理として使いまわせるように記憶する
		IfIterator<MagicsData> next = itr + 1;
		for (IfIterator<MagicsData> itr2 = next; itr2 != magics->end(); ++itr2) {
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

	for (IfIterator<MagicsData> magic_itr(*magics, live_check); magic_itr != magics->end(); ++magic_itr) {
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
