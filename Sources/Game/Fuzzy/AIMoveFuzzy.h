#pragma once
#ifndef AI_MOVE_FUZZY_DEFINED
#define AI_MOVE_FUZZY_DEFINED


#include <Utils\FuzzyObject.h>


/// <summary>
/// AI移動コマンド用入力クリスプデータ
/// </summary>
struct AIMoveInputCrisp {
	// コンストラクタ
	AIMoveInputCrisp() = default;
	AIMoveInputCrisp(float _hpGap, float _sp, float _distance, bool _lookingOther, int _hasElementNum, float _elementDitance) 
		: hpGap(_hpGap)
		, sp(_sp)
		, distance(_distance)
		, lookingOther(_lookingOther)
		, hasElementNum(_hasElementNum)
		, elementDistance(_elementDitance) {
	}

	// 最も近い敵プレイヤーとのHPの差
	float hpGap;
	// SP
	float sp;
	// 最も近い敵プレイヤーとの距離
	float distance;
	// 最も近い敵プレイヤーの方を向いているかどうか
	bool lookingOther;
	// 所持しているエレメントの数
	int hasElementNum;
	// 最も近いエレメントとの距離
	float elementDistance;
};


/// <summary>
/// AI移動コマンド用出力クリスプデータ
/// </summary>
struct AIMoveOutputCrisp {
	// ステート
	enum class State {
		// エレメントを収集する
		CollectElement,
		// 敵プレイヤーを追いかける
		Chase,
		// 敵プレイヤーから逃げる
		Evade,
	};
	// 選択したステート
	State state;
};



/// <summary>
/// 移動AI用ファジークラス
/// </summary>
class AIMoveFuzzy : public FuzzyObject<AIMoveInputCrisp, AIMoveOutputCrisp> {
public:
	// 入力から出力を計算する
	void Execute() override;

};


#endif // !AI_MOVE_FUZZY_DEFINED
