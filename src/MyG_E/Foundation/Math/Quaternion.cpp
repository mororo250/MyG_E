#include "Quaternion.h"

Quaternion::Quaternion(const float scale, const Vector<float, 3>& vector)
	:m_scalar(scale)
	,m_vector(vector)
{
}

Quaternion& Quaternion::operator+=(const Quaternion& other)
{
	m_scalar += other.m_scalar;
	m_vector += other.m_vector;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& other)
{
	m_scalar -= other.m_scalar;
	m_vector -= other.m_vector;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
	m_scalar = m_scalar * rhs.m_scalar + Vector<float, 3>::Dot(m_vector, rhs.m_vector);
	m_vector = m_scalar * rhs.m_vector + m_vector * rhs.m_scalar + Cross(m_vector, rhs.m_vector);
	return *this;
}

Quaternion& Quaternion::operator*=(const float scalar)
{
	m_scalar *= scalar;
	m_vector *= scalar;
	return *this;
}

Matrix<float, 3, 3> Quaternion::get_rotation_matrix()
{
	return create_rotation_matrix(*this);
}

Quaternion Quaternion::make_rotate(const float angle, Vector<float, 3> axis)
{
	if (!axis.isNormalized())
		axis.Normalize();
	float half_angle = 0.5f * angle;
	return Quaternion(std::cos(half_angle), axis * std::sin(half_angle));
}

Matrix<float, 3, 3> Quaternion::create_rotation_matrix(const Quaternion& quat)
{
	return Matrix<float, 3, 3>(
		{	  {1.0f - 2.0f*(std::sqrt(quat.m_vector[1]) + std::sqrt(quat.m_vector[2])), 2.0f*(quat.m_vector[0]*quat.m_vector[1] - quat.m_scalar*quat.m_vector[2]), 2.0f*(quat.m_vector[0]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[1]), 0.0f}
			, {2.0f*(quat.m_vector[0]*quat.m_vector[1] + quat.m_scalar*quat.m_vector[2]), 1.0f - 2.0f*(std::sqrt(quat.m_vector[0]) + std::sqrt(quat.m_vector[2])), 2.0f*(quat.m_vector[1]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[0]), 0.0f}
			, {2.0f*(quat.m_vector[0]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[1]), 2.0f*(quat.m_vector[1]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[0]), 1.0f - 2.0f*(std::sqrt(quat.m_vector[0]) + std::sqrt(quat.m_vector[1])), 0.0f}
		});
}

Matrix<float, 3, 3> Quaternion::create_rotation_matrix(const float angle, const Vector<float, 3>& axis)
{
	return create_rotation_matrix(make_rotate(angle, axis));
}

Vector<float, 3> Quaternion::Rotate(const Quaternion& quat, Vector<float, 3> vector)
{
	// Reference for the optimized implementation:
	//
	//   https://fgiesen.wordpress.com/2019/02/09/rotating-a-single-vector-using-a-quaternion/
	//
	const Vector<float, 3> temp = 2.0f * Cross(quat.m_vector, vector);
	return vector + quat.m_scalar * temp + Cross(quat.m_vector, temp);
}
