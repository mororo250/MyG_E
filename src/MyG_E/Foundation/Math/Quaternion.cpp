#include "Quaternion.h"

Quaternion::Quaternion(float const scale, Vector3f const& vector)
	: m_scalar(scale)
	, m_vector(vector)
{
}

float Quaternion::length() const
{
	return std::sqrt(m_scalar * m_scalar + m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] + m_vector[2] * m_vector[2]);
}

Quaternion Quaternion::get_normalized() const
{
	return Quaternion(m_scalar/ length(), m_vector/length());
}

void Quaternion::normalize()
{
	m_scalar = m_scalar / length();
	m_vector = m_vector / length();
}

Quaternion Quaternion::operator+=(Quaternion const& other)
{
	m_scalar += other.m_scalar;
	m_vector += other.m_vector;
	return *this;
}

Quaternion Quaternion::operator-=(Quaternion const& other)
{
	m_scalar -= other.m_scalar;
	m_vector -= other.m_vector;
	return *this;
}

Quaternion Quaternion::operator*=(Quaternion const& rhs)
{
	float scalar = m_scalar;
	m_scalar = m_scalar * rhs.m_scalar - Vector3f::dot(m_vector, rhs.m_vector);
	m_vector = scalar * rhs.m_vector + m_vector * rhs.m_scalar + Vector3f::cross(m_vector, rhs.m_vector);
	return *this;
}

Quaternion Quaternion::operator*=(float const scalar)
{
	m_scalar *= scalar;
	m_vector *= scalar;
	return *this;
}

Matrix4x4f Quaternion::get_rotation_matrix()
{
	return make_rotation_matrix(*this);
}

Quaternion Quaternion::make_rotate(float const angle, Vector3f axis)
{
	if (!axis.is_normalized())
		axis.normalize();
	float half_angle = 0.5f * angle;
	return Quaternion(std::cos(half_angle), axis * std::sin(half_angle));
}

constexpr Matrix4x4f Quaternion::make_rotation_matrix(Quaternion const& quat)
{
	return Matrix4x4f(	
		1.0f - 2.0f*(quat.m_vector[1]*quat.m_vector[1] + quat.m_vector[2]*quat.m_vector[2]), 2.0f*(quat.m_vector[0]*quat.m_vector[1] - quat.m_scalar*quat.m_vector[2]), 2.0f*(quat.m_vector[0]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[1]), 
		0.0f, 2.0f*(quat.m_vector[0]*quat.m_vector[1] + quat.m_scalar*quat.m_vector[2]), 1.0f - 2.0f*(quat.m_vector[0]*quat.m_vector[0] + quat.m_vector[2]*quat.m_vector[2]), 2.0f*(quat.m_vector[1]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[0]), 0.0f, 
		2.0f*(quat.m_vector[0]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[1]), 2.0f*(quat.m_vector[1]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[0]), 1.0f - 2.0f*(quat.m_vector[0]*quat.m_vector[0] + quat.m_vector[1]*quat.m_vector[1]), 
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4f Quaternion::make_rotation_matrix(float const angle, Vector3f const& axis)
{
	return make_rotation_matrix(make_rotate(angle, axis));
}

Vector3f Quaternion::rotate(Quaternion const& quat, Vector3f vector)
{
	// Reference for the optimized implementation:
	//
	//   https://fgiesen.wordpress.com/2019/02/09/rotating-a-single-vector-using-a-quaternion/
	//
	const Vector3f temp = 2.0f * Vector3f::cross(quat.m_vector, vector);
	return vector + quat.m_scalar * temp + Vector3f::cross(quat.m_vector, temp);
}

Vector3f Quaternion::ToEulerAngles(Quaternion const& quat)
{
	return Vector3f (
			std::atan2(2.0f*(quat.m_scalar*quat.m_vector[0] + quat.m_vector[1]*quat.m_vector[2]), 1.0f - 2.0f*(quat.m_vector[0]*quat.m_vector[0] + quat.m_vector[1]*quat.m_vector[1])),
			std::asin(2.0f*(quat.m_scalar*quat.m_vector[1] - quat.m_vector[2]*quat.m_vector[0])),
			std::atan2(2.0f*(quat.m_scalar*quat.m_vector[2] + quat.m_vector[0]*quat.m_vector[1]), 1.0f - 2.0f*(quat.m_vector[1]*quat.m_vector[1] + quat.m_vector[2]*quat.m_vector[2]))
		);
}

std::ostream& operator<<(std::ostream& os, Quaternion const& quat)
{
	os << "{ " << quat.m_scalar;
	os << ", " << quat.m_vector[0];
	os << ", " << quat.m_vector[1];
	os << ", " << quat.m_vector[2] << " }";
	return os;
}
