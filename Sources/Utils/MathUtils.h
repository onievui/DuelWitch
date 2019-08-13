#pragma once
#ifndef MATH_UTILS_DEFINED
#define MATH_UTILS_DEFINED


class Math {
public:
	static constexpr float PI = 3.141592654f;
	static constexpr float PI2 = 6.283185307f;
	static constexpr float HarfPI = 1.570796327f;
	static constexpr float QuarterPI = 0.785398163f;
	
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
		axis.Normalize();
		float angle = std::acosf(vec1.Dot(vec2) / (vec1.Length()*vec2.Length()));
		return DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, angle);
	}
};


#endif // !MATH_UTILS_DEFINED
