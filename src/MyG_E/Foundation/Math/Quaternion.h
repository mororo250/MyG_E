#pragma once

#include "Foundation\Math\Vector.h"
#include "Foundation\Math\Matrix.h"

class Quaternion
{
	Quaternion(const float scalar, const Vector<float, 3>& vector);
	~Quaternion();

	inline void Conjugate() { m_vector = -m_vector; }

	// Aritimetics.
	Quaternion operator+=(const Quaternion& other);
	Quaternion operator-=(const Quaternion& other);
	Quaternion operator*=(const Quaternion& other);
	Quaternion operator*=(const float scalar);

	// Return Rotation Matrix.
	Matrix<float, 4, 4> get_rotation_matrix(const Vector<float, 3>& vector);

	// Rotation Quartenion with an exis and an angle.
	static Quaternion make_rotate(const float angle, Vector<float, 3> axis);

	static Matrix<float, 4, 4> create_rotation_matrix(const Quaternion& quat);	
	static Matrix<float, 4, 4> create_rotation_matrix(const float angle, const Vector<float, 3>& axis);	
	

private:
	float m_scalar;
	Vector<float, 3> m_vector;
};

inline Quaternion operator-(Quaternion lhs, const Quaternion& rhs) { return lhs -= rhs; }
inline Quaternion operator+(Quaternion lhs, const Quaternion& rhs) { return lhs += rhs; }
inline Quaternion operator*(Quaternion lhs, const Quaternion& rhs) { return lhs *= rhs; }
inline Quaternion operator*(Quaternion lhs, float rhs) { return lhs *= rhs; }
inline Quaternion operator*(float lhs, Quaternion rhs) { return rhs *= lhs; }