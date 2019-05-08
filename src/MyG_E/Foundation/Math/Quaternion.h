#pragma once

#include "Foundation\Math\Vector.h"
#include "Foundation\Math\Matrix.h"

class Quaternion
{
  public:
	Quaternion() = default;
	Quaternion(const float scalar, const Vector<float, 3>& vector);
	~Quaternion() = default;

	inline float GetScalar() const { return m_scalar; }
	inline const Vector<float, 3>& GetVector() const { return m_vector; }

	inline void Conjugate() { m_vector = -m_vector; }
	float Length();
	void Normalize();

	// Aritimetics.
	Quaternion& operator+=(const Quaternion& other);
	Quaternion& operator-=(const Quaternion& other);
	Quaternion& operator*=(const Quaternion& other);
	Quaternion& operator*=(const float scalar);

	// Return Rotation Matrix.
	Matrix<float, 4, 4> get_rotation_matrix();

	// Rotation Quartenion with an exis and an angle.
	static Quaternion make_rotate(const float angle, Vector<float, 3> axis);
	// Return rotation Matrix
	static Matrix<float, 4, 4> create_rotation_matrix(const Quaternion& quat);	
	static Matrix<float, 4, 4> create_rotation_matrix(const float angle, const Vector<float, 3>& axis);	
	// Rotate Vector
	static Vector<float, 3> rotate(const Quaternion& quat, Vector<float, 3> vector);

	static Vector<float, 3> to_euler_angles(const Quaternion& quat);

	friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat);
private:
	float m_scalar;
	Vector<float, 3> m_vector;
};

inline Quaternion operator-(Quaternion lhs, const Quaternion& rhs) { return lhs -= rhs; }
inline Quaternion operator+(Quaternion lhs, const Quaternion& rhs) { return lhs += rhs; }
inline Quaternion operator*(Quaternion lhs, const Quaternion& rhs) { return lhs *= rhs; }
inline Quaternion operator*(Quaternion& lhs, float rhs) { return lhs *= rhs; }
inline Quaternion operator*(float lhs, Quaternion& rhs) { return rhs *= lhs; }

std::ostream& operator<<(std::ostream& os, const Quaternion& quat);