#pragma once
#ifndef FUZZY_UTILS_DEFINED
#define FUZZY_UTILS_DEFINED


/// <summary>
/// �t�@�W�[�֌W�N���X
/// </summary>
class Fuzzy {
public:
	// �u�[�������o�[�V�b�v�֐�
	static constexpr float Bool(float value, float x0) {
		return value >= x0;
	}
	// �X�΁i���j�����o�[�V�b�v�֐�
	static constexpr float Grade(float value, float x0, float x1) {
		return value <= x0 ? 0 : value >= x1 ? 1 : (value - x0) / (x1 - x0);
	}
	// �X�΁i����j�����o�[�V�b�v�֐�
	static constexpr float ReverseGrade(float value, float x0, float x1) {
		return value <= x0 ? 1 : value >= x1 ? 0 : (x1 - value) / (x1 - x0);
	}
	// �O�p�`�����o�[�V�b�v�֐�
	static constexpr float Triangle(float value, float x0, float x1, float x2) {
		return (value <= x0 || value >= x2) ? 0.0f :
			(value < x1) ? (value - x0) / (x1 - x0) : (x2 - value) / (x2 - x1);
	}
	// ��`�����o�[�V�b�v�֐�
	static constexpr float Trapezoid(float value, float x0, float x1, float x2, float x3) {
		return (value <= x0 || value >= x3) ? 0.0f :
			(value >= x1 && value <= x2) ? 1.0f :
			(value < x1) ? (value - x0) / (x1 - x0) : (x3 - value) / (x3 - x2);
	}

	// 2��w�b�W�֐�
	static constexpr float Very(float value) {
		return value * value;
	}
	// 0.5��w�b�W�֐�
	static float NotVery(float value) {
		return std::powf(value, 0.5f);
	}

	// �_���a
	static constexpr float Or(float a, float b) {
		return std::max(a, b);
	}
	// �_����
	static constexpr float And(float a, float b) {
		return std::min(a, b);
	}
	// �_���ے�
	static constexpr float Not(float a) {
		return 1.0f - a;
	}
};

#endif // !FUZZY_UTILS_DEFINED
