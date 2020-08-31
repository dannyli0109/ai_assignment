#pragma once
#include <math.h>

namespace Math {
	struct Vector3 {
		float x, y, z;
		Vector3() = default;
		Vector3(float a, float b, float c) {
			x = a;
			y = b;
			z = c;
		}

		float& operator[](int i) {
			return (&x)[i];
		}

		const float& operator[](int i) const {
			return (&x)[i];
		}

		Vector3& operator *=(float s) {
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		Vector3& operator /=(float s) {
			s = 1.0f / s;
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		Vector3& operator +=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3& operator -=(const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
	};

	inline Vector3 operator *(const Vector3& v, float s) {
		return Vector3(v.x * s, v.y * s, v.z * s);
	}

	inline Vector3 operator /(const Vector3& v, float s) {
		s = 1.0f / s;
		return Vector3(v.x * s, v.y * s, v.z * s);
	}

	inline Vector3 operator -(const Vector3& v) {
		return Vector3(-v.x, -v.y, -v.z);
	}

	inline float Magnitude(const Vector3& v) {
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline Vector3 Normalise(const Vector3& v) {
		float mag = Magnitude(v);
		if (mag == 0) return { 0, 0, 0 };
		return v / mag;
	}

	inline Vector3 operator +(const Vector3& a, const Vector3& b) {
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	inline Vector3 operator -(const Vector3& a, const Vector3& b) {
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	inline float Dot(const Vector3& a, const Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline Vector3 Cross(const Vector3& a, const Vector3& b) {
		return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	inline Vector3 Project(const Vector3& a, const Vector3& b) {
		return b * (Dot(a, b) / Dot(b, b));
	}

	inline Vector3 Reject(const Vector3& a, const Vector3& b) {
		return a - b * (Dot(a, b) / Dot(b, b));
	}
}
