#pragma once

#include "Vector3.h"

namespace Math {
	struct Matrix3
	{
	private:
		float n[3][3];
	public:
		Matrix3()
		{
			n[0][0] = 1; n[0][1] = 0; n[0][2] = 0;
			n[1][0] = 0; n[1][1] = 1; n[1][2] = 0;
			n[2][0] = 0; n[2][1] = 0; n[2][2] = 1;
		}

		Matrix3(float n00, float n01, float n02,
			float n10, float n11, float n12,
			float n20, float n21, float n22)
		{
			n[0][0] = n00; n[0][1] = n10; n[0][2] = n20;
			n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
			n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;
		}

		Matrix3(const Vector3& a, const Vector3& b, const Vector3& c)
		{
			n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z;
			n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z;
			n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z;
		}

		float& operator ()(int i, int j)
		{
			return n[j][i];
		}

		const float& operator ()(int i, int j) const {
			return n[j][i];
		}

		Vector3& operator [](int j) {
			return *reinterpret_cast<Vector3*>(n[j]);
		}

		const Vector3& operator [](int j) const {
			return *reinterpret_cast<const Vector3*>(n[j]);
		}

		void Set(const Matrix3& m) {
			(*this)(0, 0) = m(0, 0); (*this)(0, 1) = m(0, 1); (*this)(0, 2) = m(0, 2);
			(*this)(1, 0) = m(1, 0); (*this)(1, 1) = m(1, 1); (*this)(1, 2) = m(1, 2);
			(*this)(2, 0) = m(2, 0); (*this)(2, 1) = m(2, 1); (*this)(2, 2) = m(2, 2);
		}

		void SetRotationX(float t) {
			float c = cos(t);
			float s = sin(t);

			Set(Matrix3(
				1.0f, 0.0f, 0.0f,
				0.0f, c, -s,
				0.0f, s, c
			));
		}

		void SetRotationY(float t) {
			float c = cos(t);
			float s = sin(t);

			Set(Matrix3(
				c, 0.0f, s,
				0.0f, 1.0f, 0.0f,
				-s, 0.0f, c
			));
		}

		void SetRotationZ(float t) {
			float c = cos(t);
			float s = sin(t);

			Set(Matrix3(
				c, -s, 0.0f,
				s, c, 0.0f,
				0.0f, 0.0f, 1.0f
			));
		}

		void SetRotation(float t, const Vector3& a) {
			float c = cos(t);
			float s = sin(t);
			float d = 1.0f - c;

			float x = a.x * d;
			float y = a.y * d;
			float z = a.z * d;
			float axay = x * a.y;
			float axaz = x * a.z;
			float ayaz = y * a.z;

			Set(Matrix3(
				c + x * a.x, axay - s * a.z, axaz + s * a.y,
				axay + s * a.z, c + y * a.y, ayaz - s * a.x,
				axaz - s * a.y, ayaz + s * a.x, c + z * a.z
			));
		}

		void Translate(float x, float y)
		{
			(*this)(0, 2) += x;
			(*this)(1, 2) += y;
		}

	};

	inline Matrix3 operator *(const Matrix3& a, const Matrix3& b) {
		return Matrix3(
			a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0) + a(0, 2) * b(2, 0),
			a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1) + a(0, 2) * b(2, 1),
			a(0, 0) * b(0, 2) + a(0, 1) * b(1, 2) + a(0, 2) * b(2, 2),
			a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0) + a(1, 2) * b(2, 0),
			a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1) + a(1, 2) * b(2, 1),
			a(1, 0) * b(0, 2) + a(1, 1) * b(1, 2) + a(1, 2) * b(2, 2),
			a(2, 0) * b(0, 0) + a(2, 1) * b(1, 0) + a(2, 2) * b(2, 0),
			a(2, 0) * b(0, 1) + a(2, 1) * b(1, 1) + a(2, 2) * b(2, 1),
			a(2, 0) * b(0, 2) + a(2, 1) * b(1, 2) + a(2, 2) * b(2, 2)
		);
	}

	inline Vector3 operator *(const Matrix3& m, const Vector3& v) {
		return Vector3(
			m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z,
			m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z,
			m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z
		);
	}

	inline Matrix3 Inverse(const Matrix3& m) {
		const Vector3& a = m[0];
		const Vector3& b = m[1];
		const Vector3& c = m[2];

		Vector3 r0 = Cross(b, c);
		Vector3 r1 = Cross(c, a);
		Vector3 r2 = Cross(a, b);

		float invDet = 1.0f / Dot(r2, c);

		return Matrix3(
			r0.x * invDet, r0.y * invDet, r0.z * invDet,
			r1.x * invDet, r1.y * invDet, r1.z * invDet,
			r2.x * invDet, r2.y * invDet, r2.z * invDet
		);
	}
}
