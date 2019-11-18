#pragma once

#include "Foundation\Math\Vector.h"
#include "Foundation\Math\Matrix.h"

class Quaternion
{
  public:
	Quaternion() = default;
	Quaternion(float const scalar, Vector3f const& vector);
	~Quaternion() = default;

	inline float get_scalar() const { return m_scalar; }
	inline Vector3f get_Vector() const { return m_vector; }

	inline void conjugate() { m_vector = -m_vector; }
	float length() const;
	Quaternion get_normalized() const;
	void normalize();

	// Aritimetics.
	Quaternion operator+=(Quaternion const& other);
	Quaternion operator-=(Quaternion const& other);
	Quaternion operator*=(Quaternion const& other);
	Quaternion operator*=(float const scalar);

	// Return Rotation Matrix.
	Matrix4x4f get_rotation_matrix();

	// Rotation Quartenion with an exis and an angle.
	static Quaternion make_rotate(float const angle, Vector3f axis);
	// Return rotation Matrix
	static constexpr Matrix4x4f make_rotation_matrix(Quaternion const& quat);
	static Matrix4x4f make_rotation_matrix(float const angle, Vector3f const& axis);
	// Rotate Vector
	static Vector3f rotate(Quaternion const& quat, Vector3f vector);

	static Vector3f ToEulerAngles(Quaternion const& quat);

	friend std::ostream& operator<<(std::ostream& os, Quaternion const& quat);
private:

	float m_scalar;
	Vector3f m_vector;
};

inline Quaternion operator-(Quaternion lhs, Quaternion const& rhs) { return lhs -= rhs; }
inline Quaternion operator+(Quaternion lhs, Quaternion const& rhs) { return lhs += rhs; }
inline Quaternion operator*(Quaternion lhs, Quaternion const& rhs) { return lhs *= rhs; }
inline Quaternion operator*(Quaternion lhs, float const rhs) { return lhs *= rhs; }
inline Quaternion operator*(float const lhs, Quaternion& rhs) { return rhs *= lhs; }

std::ostream& operator<<(std::ostream& os, const Quaternion& quat);