#include "CollisionManager.h"
#include <Utils\LamdaUtils.h>
#include <Game\Collision\Collision.h>
#include <Game\Player\Player.h>
#include <Game\Field\Field.h>
#include <Game\Element\Element.h>
#include <Game\Magic\IMagic.h>


/// <summary>
/// �v���C���[�ƃt�B�[���h�̓����蔻����s��
/// </summary>
/// <param name="players">�v���C���[</param>
/// <param name="field">�t�B�[���h</param>
void CollisionManager::CollisionPlayerField(PlayersData* players, FieldData* field) {
	for (PlayersData::iterator itr = players->begin(); itr != players->end(); ++itr) {
		field->CollisionCheckPlayer(itr->get());
	}
}

/// <summary>
/// �v���C���[�ƃG�������g�̓����蔻����s��
/// </summary>
/// <param name="players">�v���C���[</param>
/// <param name="elements">�G�������g</param>
void CollisionManager::CollisionPlayerElement(PlayersData* players, ElementsData* elements) {
	// ���g�p�̃I�u�W�F�N�g�͔�΂��悤�ɂ���
	auto live_check = LamdaUtils::NotNull();

	for (ElementsData::iterator element_itr = LamdaUtils::FindIf(*elements, live_check),
		element_end = elements->end();
		element_itr != element_end;
		LamdaUtils::FindIfNext(element_itr, element_end, live_check)) {
		const Collider* element_collider = (*element_itr)->GetCollider();
		for (PlayersData::iterator player_itr = players->begin(); player_itr != players->end(); ++player_itr) {
			if (Collision::HitCheck(element_collider, (*player_itr)->GetCollider())) {
				// �v���C���[�ɃG�������g���擾������
				(*player_itr)->GetElement((*element_itr)->GetID());
				// �G�������g�𖢎g�p�ɂ���
				(*element_itr)->SetUsed(false);
				break;
			}
		}
	}
}

/// <summary>
/// ���@���m�̓����蔻����s��
/// </summary>
/// <param name="magics">���@</param>
void CollisionManager::CollisionMagic(MagicsData* magics) {
	// ���g�p�̃I�u�W�F�N�g�͔�΂��悤�ɂ���
	auto live_check = LamdaUtils::NotNull();

	for (MagicsData::iterator itr = LamdaUtils::FindIf(*magics, live_check),
		end = magics->end();
		itr != end;) {
		const Collider* collider = (*itr)->GetCollider();
		MagicsData::iterator next = std::find_if(itr + 1, end, live_check);
		for (MagicsData::iterator itr2 = next; itr2 != end; itr2 = std::find_if(itr2 + 1, end, live_check)) {
			// ����v���C���[�̖��@�Ȃ画�肵�Ȃ�
			if ((*itr)->GetPlayerID() == (*itr2)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(collider, (*itr2)->GetCollider())) {
				// �Փ˂������Ƃ�ʒm����
				(*itr)->HitMagic(*itr2);
				(*itr2)->HitMagic(*itr);
			}
		}
		itr = next;
	}
}

/// <summary>
/// �v���C���[�Ɩ��@�̓����蔻����s��
/// </summary>
/// <param name="players">�v���C���[</param>
/// <param name="magics">���@</param>
void CollisionManager::CollisionPlayerMagic(PlayersData* players, MagicsData* magics) {
	// ���g�p�̃I�u�W�F�N�g�͔�΂��悤�ɂ���
	auto live_check = LamdaUtils::NotNull();

	for (MagicsData::iterator magic_itr = LamdaUtils::FindIf(*magics, live_check),
		magic_end = magics->end();
		magic_itr != magic_end;
		LamdaUtils::FindIfNext(magic_itr, magic_end, live_check)) {
		const Collider* magic_collider = (*magic_itr)->GetCollider();
		for (PlayersData::iterator player_itr = players->begin(); player_itr != players->end(); ++player_itr) {
			// ���g�̖��@�Ƃ͔��肵�Ȃ�
			if ((*player_itr)->GetPlayerID() == (*magic_itr)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(magic_collider, (*player_itr)->GetCollider())) {
				// ���@�Ƀv���C���[���Փ˂������Ƃ�ʒm����
				(*magic_itr)->HitPlayer((*player_itr)->GetCollider());
				// �v���C���[�ɖ��@���Փ˂������Ƃ�ʒm����
				(*player_itr)->HitMagic(*magic_itr);
			}
		}
	}
}

/// <summary>
/// �v���C���[���m�̓����蔻����s��
/// </summary>
/// <param name="players">�v���C���[</param>
void CollisionManager::CollisionPlayer(PlayersData* players) {
	for (PlayersData::iterator itr1 = players->begin(); itr1 != players->end() - 1; ++itr1) {
		const Collider* collider1 = (*itr1)->GetCollider();
		for (PlayersData::iterator itr2 = itr1 + 1; itr2 != players->end(); ++itr2) {
			if (Collision::HitCheck(collider1, (*itr2)->GetCollider())) {
				// �Փ˂������Ƃ�ʒm����
				(*itr1)->HitPlayer(**itr2);
				(*itr2)->HitPlayer(**itr1);
			}
		}
	}
}
