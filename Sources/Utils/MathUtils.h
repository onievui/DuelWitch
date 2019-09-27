#pragma once
#ifndef MATH_UTILS_DEFINED
#define MATH_UTILS_DEFINED


/// <summary>
/// 数学関係クラス
/// </summary>
class Math {
public:
	static constexpr float PI = 3.141592654f;
	static constexpr float PI2 = 6.283185307f;
	static constexpr float HarfPI = 1.570796327f;
	static constexpr float QuarterPI = 0.785398163f;

	static constexpr float Epsilon = FLT_EPSILON;
	
	// 度数法から弧度法に変換する
	static constexpr float Deg2Rad(float deg) {
		return deg/180.0f*PI;
	}

	// 弧度法から度数法に変換する
	static constexpr float Rad2Deg(float rad) {
		return rad/PI*180.0f;
	}

	template<class T>
	// クランプ
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
	// 線形補間
	static T Lerp(const T& a, const T& b, float t) {
		return a + (b - a)*t;
	}

	// ベクトルからベクトルへのクォータニオンを生成する
	static DirectX::SimpleMath::Quaternion CreateQuaternionFromVector3(
		const DirectX::SimpleMath::Vector3& vec1,
		const DirectX::SimpleMath::Vector3& vec2) {
		DirectX::SimpleMath::Vector3 axis = vec1.Cross(vec2);
		if (axis.Length() <= 0.0f) {
			return DirectX::SimpleMath::Quaternion::Identity;
		}
		axis.Normalize();
		float angle = std::acosf(vec1.Dot(vec2) / (vec1.Length()*vec2.Length()));
		return DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, angle);
	}
};


#endif // !MATH_UTILS_DEFINED
