#pragma once
#include <math.h>

namespace Math {
	struct Vector4 {
		float x, y, z, w;
		Vector4() = default;
		Vector4(float a, float b, float c, float d) {
			x = a;
			y = b;
			z = c;
			w = d;
		}

		float& operator [](int i) {
			return (&x)[i];
		}

		const float& operator [](int i) const {
			return (&x)[i];
		}

		Vector4& operator *=(float s) {
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		Vector4& operator /=(float s) {
			s = 1.0f / s;
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		Vector4& operator +=(const Vector4& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		Vector4& operator -=(const Vector4& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}
	};

	inline Vector4 operator *(const Vector4& v, float s) {
		return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
	}

	inline Vector4 operator /(const Vector4& v, float s) {
		s = 1.0f / s;
		return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
	}

	inline Vector4 operator -(const Vector4& v) {
		return Vector4(-v.x, -v.y, -v.z, -v.w);
	}

	inline float Magnitude(const Vector4& v) {
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	}

	inline Vector4 Normalise(const Vector4& v) {
		return v / Magnitude(v);
	}

	inline Vector4 operator +(const Vector4& a, const Vector4& b) {
		return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	inline Vector4 operator -(const Vector4& a, const Vector4& b) {
		return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	inline float Dot(const Vector4& a, const Vector4& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline Vector4 Cross(const Vector4& a, const Vector4& b) {
		return Vector4(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x,
			0
		);
	}

	inline Vector4 Project(const Vector4& a, const Vector4& b) {
		return b * (Dot(a, b) / Dot(b, b));
	}

	inline Vector4 Reject(const Vector4& a, const Vector4& b) {
		return a - b * (Dot(a, b) / Dot(b, b));
	}
}
