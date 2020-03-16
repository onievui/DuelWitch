#pragma once
#ifndef MAGIC_PARAMETER_DEFINED
#define MAGIC_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// 魔法クラス用パラメータ
/// </summary>
class MagicParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

private:
	// パラメータを読み込む
	bool LoadData();

public:
	std::vector<int> maxNum;

	struct normal_param {
		float power;				// ダメージ量
		float radius;				// 半径
		float moveSpeed;			// 移動速度
		float lockOnRotateSpeed;	// ロックオン時の方向転換速度
		float lockOnTime;			// ロックオンを終了する時間
		float lifeTime;				// 消えるまでの時間
	};
	normal_param normalParam;

	struct fire_param {
		float power;	            // ダメージ量
		float wayAngle;	            // 各魔法の角度の間隔
		float radius;               // 半径
		float height;               // 高さ
		float moveSpeed;	        // 移動速度
		float lockOnRotateSpeed;	// ロックオン時の方向転換速度
		float lockOnTime;			// ロックオンを終了する時間
		float lifeTime;	            // 消えるまでの時間
	};
	fire_param fireParam;

	struct freeze_param {
		float power;                // ダメージ量
		float radius;               // 半径
		float rotateSpeed;          // 回転速度
		float rotateRadius;	        // 回転半径
		float lifeTime;             // 消えるまでの時間
	};
	freeze_param freezeParam;

	struct thunder_param {
		float power;                // ダメージ量
		float radius;               // 半径
		float colliderRadius;       // 追尾当たり判定用半径
		float moveSpeed;            // 移動速度
		float chaseSpeed;           // 追尾速度
		float chaseEndDistance;     // 追尾を終了する距離
		float chaseLerpSpeed;       // 追尾の強さ
		float chaseEndTime;         // 追尾を終了する時間
		float lockOnRotateSpeed;	// ロックオン時の方向転換速度
		float lockOnTime;			// ロックオンを終了する時間
		float lifeTime;             // 消えるまでの時間
	};
	thunder_param thunderParam;

	struct thunder_strike_param {
		float power;        // ダメージ量
		float radius;       // 半径
		float height;       // 高さ
		float appearPosY;	// 出現する高さのオフセット
		float moveSpeed;    // 移動速度
		float lifeTime;     // 消えるまでの時間
	};
	thunder_strike_param thunderStrikeParam;
};

#endif // !MAGIC_PARAMETER_DEFINED
