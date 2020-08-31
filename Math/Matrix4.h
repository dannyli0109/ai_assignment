#pragma once
#include "Vector4.h"
#include "Vector3.h"

namespace Math {
	struct Matrix4
	{
	private:
		float n[4][4];
	public:
		Matrix4() = default;
		Matrix4(float n00, float n01, float n02, float n03,
			float n10, float n11, float n12, float n13,
			float n20, float n21, float n22, float n23,
			float n30, float n31, float n32, float n33)
		{
			n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
			n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
			n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
			n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
		}

		Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d) {
			n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[0][3] = a.w;
			n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z; n[1][3] = b.w;
			n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z; n[2][3] = c.w;
			n[3][0] = d.x; n[3][1] = d.y; n[3][2] = d.z; n[3][3] = d.w;
		}

		float& operator ()(int i, int j) {
			return n[j][i];
		}

		const float& operator ()(int i, int j) const {
			return n[j][i];
		}

		Vector4& operator [](int j) {
			return *reinterpret_cast<Vector4*>(n[j]);
		}

		const Vector4& operator [](int j) const {
			return *reinterpret_cast<const Vector4*>(n[j]);
		}

		void Set(const Matrix4& m)
		{
			(*this)(0, 0) = m(0, 0); (*this)(0, 1) = m(0, 1); (*this)(0, 2) = m(0, 2); (*this)(0, 3) = m(0, 3);
			(*this)(1, 0) = m(1, 0); (*this)(1, 1) = m(1, 1); (*this)(1, 2) = m(1, 2); (*this)(1, 3) = m(1, 3);
			(*this)(2, 0) = m(2, 0); (*this)(2, 1) = m(2, 1); (*this)(2, 2) = m(2, 2); (*this)(2, 3) = m(2, 3);
			(*this)(3, 0) = m(3, 0); (*this)(3, 1) = m(3, 1); (*this)(3, 2) = m(3, 2); (*this)(3, 3) = m(3, 3);
		}

		void SetRotationX(float t) {
			float c = cos(t);
			float s = sin(t);

			Set(Matrix4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, c, -s, 0.0f,
				0.0f, s, c, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			));
		}

		void SetRotationY(float t) {
			float c = cos(t);
			float s = sin(t);

			Set(Matrix4(
				c, 0.0f, s, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				-s, 0.0f, c, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			));
		}

		void SetRotationZ(float t) {
			float c = cos(t);
			float s = sin(t);

			Set(Matrix4(
				c, -s, 0.0f, 0.0f,
				s, c, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
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

			Set(Matrix4(
				c + x * a.x, axay - s * a.z, axaz + s * a.y, 0.0f,
				axay + s * a.z, c + y * a.y, ayaz - s * a.x, 0.0f,
				axaz - s * a.y, ayaz + s * a.x, c + z * a.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			));
		}

	};

	Matrix4 operator *(const Matrix4& a, const Matrix4& b) {
		return Matrix4(
			a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0) + a(0, 2) * b(2, 0) + a(0, 3) * b(3, 0),
			a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1) + a(0, 2) * b(2, 1) + a(0, 3) * b(3, 1),
			a(0, 0) * b(0, 2) + a(0, 1) * b(1, 2) + a(0, 2) * b(2, 2) + a(0, 3) * b(3, 2),
			a(0, 0) * b(0, 3) + a(0, 1) * b(1, 3) + a(0, 2) * b(2, 3) + a(0, 3) * b(3, 3),

			a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0) + a(1, 2) * b(2, 0) + a(1, 3) * b(3, 0),
			a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1) + a(1, 2) * b(2, 1) + a(1, 3) * b(3, 1),
			a(1, 0) * b(0, 2) + a(1, 1) * b(1, 2) + a(1, 2) * b(2, 2) + a(1, 3) * b(3, 2),
			a(1, 0) * b(0, 3) + a(1, 1) * b(1, 3) + a(1, 2) * b(2, 3) + a(1, 3) * b(3, 3),

			a(2, 0) * b(0, 0) + a(2, 1) * b(1, 0) + a(2, 2) * b(2, 0) + a(2, 3) * b(3, 0),
			a(2, 0) * b(0, 1) + a(2, 1) * b(1, 1) + a(2, 2) * b(2, 1) + a(2, 3) * b(3, 1),
			a(2, 0) * b(0, 2) + a(2, 1) * b(1, 2) + a(2, 2) * b(2, 2) + a(2, 3) * b(3, 2),
			a(2, 0) * b(0, 3) + a(2, 1) * b(1, 3) + a(2, 2) * b(2, 3) + a(2, 3) * b(3, 3),

			a(3, 0) * b(0, 0) + a(3, 1) * b(1, 0) + a(3, 2) * b(2, 0) + a(3, 3) * b(3, 0),
			a(3, 0) * b(0, 1) + a(3, 1) * b(1, 1) + a(3, 2) * b(2, 1) + a(3, 3) * b(3, 1),
			a(3, 0) * b(0, 2) + a(3, 1) * b(1, 2) + a(3, 2) * b(2, 2) + a(3, 3) * b(3, 2),
			a(3, 0) * b(0, 3) + a(3, 1) * b(1, 3) + a(3, 2) * b(2, 3) + a(3, 3) * b(3, 3)
		);
	}

	Vector4 operator *(const Matrix4& m, const Vector4& v) {
		return Vector4(
			m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3) * v.w,
			m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3) * v.w,
			m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3) * v.w,
			m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3) * v.w
		);
	}

	Matrix4 Inverse(const Matrix4& m) {
		const Vector3& a = reinterpret_cast<const Vector3&>(m[0]);
		const Vector3& b = reinterpret_cast<const Vector3&>(m[1]);
		const Vector3& c = reinterpret_cast<const Vector3&>(m[2]);
		const Vector3& d = reinterpret_cast<const Vector3&>(m[3]);

		const float& x = m(3, 0);
		const float& y = m(3, 1);
		const float& z = m(3, 2);
		const float& w = m(3, 3);

		Vector3 s = Cross(a, b);
		Vector3 t = Cross(c, d);
		Vector3 u = a * y - b * x;
		Vector3 v = c * w - d * z;

		float invDet = 1.0f / (Dot(s, v) + Dot(t, u));

		s *= invDet;
		t *= invDet;
		u *= invDet;
		v *= invDet;

		Vector3 r0 = Cross(b, v) + t * y;
		Vector3 r1 = Cross(v, a) - t * x;
		Vector3 r2 = Cross(d, u) + s * w;
		Vector3 r3 = Cross(u, c) - s * z;

		return Matrix4(
			r0.x, r0.y, r0.z, -Dot(b, t),
			r1.x, r1.y, r1.z, Dot(a, t),
			r2.x, r2.y, r2.z, -Dot(d, s),
			r3.x, r3.y, r3.z, Dot(c, s)
		);
	}
}
