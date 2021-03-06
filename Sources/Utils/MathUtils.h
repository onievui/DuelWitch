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
		return deg/180.0f*Math::PI;
	}

	// 弧度法から度数法に変換する
	static constexpr float Rad2Deg(float rad) {
		return rad/Math::PI*180.0f;
	}

	// 小数がゼロかどうか調べる
	static constexpr bool Equal0(float val) {
		return (val >= -Math::Epsilon && val <= Math::Epsilon);
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

	// ベクトルのなす角を取得する
	static float BetweenAngle(const DirectX::SimpleMath::Vector3& vec1, const DirectX::SimpleMath::Vector3& vec2) {
		float length = vec1.Length()*vec2.Length();
		if (length > Math::Epsilon) {
			return std::acosf(vec1.Dot(vec2) / length);
		}
		else {
			return 0.0f;
		}
	}

	// あるベクトルに垂直なベクトルを生成する
	static DirectX::SimpleMath::Vector3 CreateNormalVector3(const DirectX::SimpleMath::Vector3& vec) {
		if (Math::Equal0(vec.Length())) {
			// ゼロベクトルの場合はゼロベクトルを返す
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

	// ベクトルからベクトルへのクォータニオンを生成する
	static DirectX::SimpleMath::Quaternion CreateQuaternionFromVector3(
		const DirectX::SimpleMath::Vector3& vec1,
		const DirectX::SimpleMath::Vector3& vec2) {
		DirectX::SimpleMath::Vector3 axis = vec1.Cross(vec2);
		// 平行かどうか調べる
		if (Math::Equal0(axis.Length())) {
			// 同じ向きの場合
			if (vec1.Dot(vec2) > 0.0f) {
				return DirectX::SimpleMath::Quaternion::Identity;
			}
			// 逆向きの場合
			return DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(CreateNormalVector3(vec1), Math::PI);
		}
		axis.Normalize();

		//float angle = BetweenAngle(vec1, vec2);
		//return DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, angle);

		float c = vec1.Dot(vec2) / (vec1.Length()*vec2.Length());
		float cos_half = std::sqrtf((1 + c)*0.5f);
		float sin_half = std::sqrtf((1 - c)*0.5f);
		return DirectX::SimpleMath::Quaternion(axis.x*sin_half, axis.y*sin_half, axis.z*sin_half, cos_half);
	}
};


#endif // !MATH_UTILS_DEFINED
