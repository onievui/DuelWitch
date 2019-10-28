#pragma once
#ifndef AI_MOVE_FUZZY_DEFINED
#define AI_MOVE_FUZZY_DEFINED


#include <Utils\FuzzyObject.h>


/// <summary>
/// AI�ړ��R�}���h�p���̓N���X�v�f�[�^
/// </summary>
struct AIMoveInputCrisp {
	// �R���X�g���N�^
	AIMoveInputCrisp() = default;
	AIMoveInputCrisp(float _hpGap, float _sp, float _distance, bool _lookingOther, int _hasElementNum, float _elementDitance) 
		: hpGap(_hpGap)
		, sp(_sp)
		, distance(_distance)
		, lookingOther(_lookingOther)
		, hasElementNum(_hasElementNum)
		, elementDistance(_elementDitance) {
	}

	// �ł��߂��G�v���C���[�Ƃ�HP�̍�
	float hpGap;
	// SP
	float sp;
	// �ł��߂��G�v���C���[�Ƃ̋���
	float distance;
	// �ł��߂��G�v���C���[�̕��������Ă��邩�ǂ���
	bool lookingOther;
	// �������Ă���G�������g�̐�
	int hasElementNum;
	// �ł��߂��G�������g�Ƃ̋���
	float elementDistance;
};


/// <summary>
/// AI�ړ��R�}���h�p�o�̓N���X�v�f�[�^
/// </summary>
struct AIMoveOutputCrisp {
	// �X�e�[�g
	enum class State {
		// �G�������g�����W����
		CollectElement,
		// �G�v���C���[��ǂ�������
		Chase,
		// �G�v���C���[���瓦����
		Evade,
	};
	// �I�������X�e�[�g
	State state;
};



/// <summary>
/// �ړ�AI�p�t�@�W�[�N���X
/// </summary>
class AIMoveFuzzy : public FuzzyObject<AIMoveInputCrisp, AIMoveOutputCrisp> {
public:
	// ���͂���o�͂��v�Z����
	void Execute() override;

};


#endif // !AI_MOVE_FUZZY_DEFINED
