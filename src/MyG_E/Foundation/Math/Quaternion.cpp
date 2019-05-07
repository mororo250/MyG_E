#include "Quaternion.h"

Quaternion::Quaternion(const float scale, const Vector<float, 3>& vector)
	:m_scalar(scale)
	,m_vector(vector)
{
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator+=(const Quaternion& other)
{
	m_scalar += other.m_scalar;
	m_vector += other.m_vector;
	return *this;
}

Quaternion Quaternion::operator-=(const Quaternion& other)
{
	m_scalar -= other.m_scalar;
	m_vector -= other.m_vector;
	return *this;
}

Quaternion Quaternion::operator*=(const Quaternion& rhs)
{
	m_scalar = m_scalar * rhs.m_scalar + Vector<float, 3>::Dot(m_vector, rhs.m_vector);
	m_vector = m_scalar * rhs.m_vector + m_vector * rhs.m_scalar + Cross(m_vector, rhs.m_vector);
}

Quaternion Quaternion::operator*=(const float scalar)
{
	m_scalar *= scalar;
	m_vector *= scalar;
	return *this;
}

Matrix<float, 4, 4> Quaternion::get_rotation_matrix(const Vector<float, 3>& vector)
{
	return create_rotation_matrix(*this);
}

Quaternion Quaternion::make_rotate(const float angle, Vector<float, 3> axis)
{
	if (!axis.isNormalized())
		axis.Normalize()&
	float half_angle = 0.5f * angle;
	return Quaternion(std::cos(half_angle), axis * std::sin(half_angle));
}

Matrix<float, 4, 4> Quaternion::create_rotation_matrix(const Quaternion& quat)
{
	return Matrix<float, 4, 4>(
		{	  {1.0f - 2.0f*(std::sqrt(quat.m_vector[1]) + std::sqrt(quat.m_vector[2])), 2.0f*(quat.m_vector[0]*quat.m_vector[1] - *quat.m_scalar*quat.m_vector[2]), 2.0f*(quat.m_vector[0]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[1]), 0.0f}
			, {2.0f*(quat.m_vector[0]*quat.m_vector[1] + quat.m_scalar*quat.m_vector[2]), 1.0f - 2.0f*(std::sqrt(quat.m_vector[0]) + std::sqrt(quat.m_vector[2])), 2.0f*(quat.m_vector[1]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[0]), 0.0f}
			, {2.0f*(quat.m_vector[0]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[1]), 2.0f*(quat.m_vector[1]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[0]), 1.0f - 2.0f*(std::sqrt(quat.m_vector[0]) + std::sqrt(quat.m_vector[1])), 0.0f}
			, {0.0f, 0.0f, 0.0f, 1.0f}});
}

Matrix<float, 4, 4> Quaternion::create_rotation_matrix(const float angle, const Vector<float, 3>& axis)
{
	create_rotation_matrix(make_rotate(angle, axis));
}
