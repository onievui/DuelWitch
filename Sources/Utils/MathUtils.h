#pragma once
#ifndef MATH_UTILS_DEFINED
#define MATH_UTILS_DEFINED


/// <summary>
/// ���w�֌W�N���X
/// </summary>
class Math {
public:
	static constexpr float PI = 3.141592654f;
	static constexpr float PI2 = 6.283185307f;
	static constexpr float HarfPI = 1.570796327f;
	static constexpr float QuarterPI = 0.785398163f;

	static constexpr float Epsilon = FLT_EPSILON;
	
	// �x���@����ʓx�@�ɕϊ�����
	static constexpr float Deg2Rad(float deg) {
		return deg/180.0f*Math::PI;
	}

	// �ʓx�@����x���@�ɕϊ�����
	static constexpr float Rad2Deg(float rad) {
		return rad/Math::PI*180.0f;
	}

	// �������[�����ǂ������ׂ�
	static constexpr bool Equal0(float val) {
		return (val >= -Math::Epsilon && val <= Math::Epsilon);
	}


	template<class T>
	// �N�����v
	static T Clamp(const T& value, const T& min, const T& max) {
		return (value < min ? min : value > max ? max : value);
	}

	template<>
	static DirectX::SimpleMath::Vector2 Clamp(const DirectX::SimpleMath::Vector2& value, const DirectX::SimpleMath::Vector2& min,
		const DirectX::SimpleMath::Vector2& max) {
		return DirectX::SimpleMath::Vector2(
			Clamp(value.x, min.x, max.x),
			Clamp(value.y, min.y, max.y)
		);
	}

	template<>
	static DirectX::SimpleMath::Vector3 Clamp(const DirectX::SimpleMath::Vector3& value, const DirectX::SimpleMath::Vector3& min,
		const DirectX::SimpleMath::Vector3& max) {
		return DirectX::SimpleMath::Vector3(
			Clamp(value.x, min.x, max.x),
			Clamp(value.y, min.y, max.y),
			Clamp(value.z, min.z, max.z)
		);
	}

	template<class T>
	// ���`���
	static T Lerp(const T& a, const T& b, float t) {
		return a + (b - a)*t;
	}

	// ����x�N�g���ɐ����ȃx�N�g���𐶐�����
	static DirectX::SimpleMath::Vector3 CreateNormalVector3(const DirectX::SimpleMath::Vector3& vec) {
		if (Math::Equal0(vec.Length())) {
			// �[���x�N�g���̏ꍇ�̓[���x�N�g����Ԃ�
			return DirectX::SimpleMath::Vector3::Zero;
		}
		if (Math::Equal0(vec.x)) {
			return DirectX::SimpleMath::Vector3(1, 0, 0);
		}
		if (Math::Equal0(vec.y)) {
			return DirectX::SimpleMath::Vector3(0, 1, 0);
		}
		return DirectX::SimpleMath::Vector3(1, -vec.x / vec.y, 0);
	}

	// �x�N�g������x�N�g���ւ̃N�H�[�^�j�I���𐶐�����
	static DirectX::SimpleMath::Quaternion CreateQuaternionFromVector3(
		const DirectX::SimpleMath::Vector3& vec1,
		const DirectX::SimpleMath::Vector3& vec2) {
		DirectX::SimpleMath::Vector3 axis = vec1.Cross(vec2);
		if (Math::Equal0(axis.Length())) {
			// �����x�N�g���̏ꍇ
			if (Math::Equal0(vec1.Dot(vec2))) {
				return DirectX::SimpleMath::Quaternion::Identity;
			}
			// �t�x�N�g���̏ꍇ
			return DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(CreateNormalVector3(vec1), Math::PI);
		}
		axis.Normalize();
		float angle = std::acosf(vec1.Dot(vec2) / (vec1.Length()*vec2.Length()));
		return DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, angle);
	}
};


#endif // !MATH_UTILS_DEFINED
