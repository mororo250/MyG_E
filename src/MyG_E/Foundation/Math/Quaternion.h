#pragma once

#include "Foundation\Math\Vector.h"
#include "Foundation\Math\Matrix.h"

class Quaternion
{
  public:
	Quaternion() = default;
	Quaternion(const float scalar, const Vector3f& vector);
	~Quaternion() = default;

	inline float GetScalar() const { return m_scalar; }
	inline const Vector3f& GetVector() const { return m_vector; }

	inline void Conjugate() { m_vector = -m_vector; }
	float Length();
	void Normalize();

	// Aritimetics.
	Quaternion& operator+=(const Quaternion& other);
	Quaternion& operator-=(const Quaternion& other);
	Quaternion& operator*=(const Quaternion& other);
	Quaternion& operator*=(const float scalar);

	// Return Rotation Matrix.
	Matrix<float, 4, 4> GetRotationMatrix();

	// Rotation Quartenion with an exis and an angle.
	static Quaternion MakeRotate(const float angle, Vector3f axis);
	// Return rotation Matrix
	static Matrix<float, 4, 4> CreateRotationMatrix(const Quaternion& quat);	
	static Matrix<float, 4, 4> CreateRotationMatrix(const float angle, const Vector3f& axis);	
	// Rotate Vector
	static Vector3f rotate(const Quaternion& quat, Vector3f vector);

	static Vector3f ToEulerAngles(const Quaternion& quat);

	friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat);
private:
	float m_scalar;
	Vector3f m_vector;
};

inline Quaternion operator-(Quaternion lhs, const Quaternion& rhs) { return lhs -= rhs; }
inline Quaternion operator+(Quaternion lhs, const Quaternion& rhs) { return lhs += rhs; }
inline Quaternion operator*(Quaternion lhs, const Quaternion& rhs) { return lhs *= rhs; }
inline Quaternion operator*(Quaternion& lhs, float rhs) { return lhs *= rhs; }
inline Quaternion operator*(float lhs, Quaternion& rhs) { return rhs *= lhs; }

std::ostream& operator<<(std::ostream& os, const Quaternion& quat);