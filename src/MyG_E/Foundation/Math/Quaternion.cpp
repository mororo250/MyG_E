#include "Quaternion.h"

Quaternion::Quaternion(const float scale, const Vector<float, 3>& vector)
	: m_scalar(scale)
	, m_vector(vector)
{
}

float Quaternion::Length()
{
	return std::sqrt(m_scalar * m_scalar + m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] + m_vector[2] * m_vector[2]);
}

void Quaternion::Normalize()
{
	m_scalar = m_scalar / Length();
	m_vector[0] = m_vector[0] / Length();
	m_vector[1] = m_vector[1] / Length();
	m_vector[2] = m_vector[2] / Length();
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
	float scalar = m_scalar;
	m_scalar = m_scalar * rhs.m_scalar - Vector<float, 3>::Dot(m_vector, rhs.m_vector);
	m_vector = scalar * rhs.m_vector + m_vector * rhs.m_scalar + Cross(m_vector, rhs.m_vector);
	return *this;
}

Quaternion& Quaternion::operator*=(const float scalar)
{
	m_scalar *= scalar;
	m_vector *= scalar;
	return *this;
}

Matrix<float, 4, 4> Quaternion::GetRotationMatrix()
{
	return CreateRotationMatrix(*this);
}

Quaternion Quaternion::MakeRotate(const float angle, Vector<float, 3> axis)
{
	if (!axis.isNormalized())
		axis.Normalize();
	float half_angle = 0.5f * angle;
	return Quaternion(std::cos(half_angle), axis * std::sin(half_angle));
}

Matrix<float, 4, 4> Quaternion::CreateRotationMatrix(const Quaternion& quat)
{
	return Matrix<float, 4, 4>(
		{	  {1.0f - 2.0f*(quat.m_vector[1]*quat.m_vector[1] + quat.m_vector[2]*quat.m_vector[2]), 2.0f*(quat.m_vector[0]*quat.m_vector[1] - quat.m_scalar*quat.m_vector[2]), 2.0f*(quat.m_vector[0]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[1]), 0.0f}
			, {2.0f*(quat.m_vector[0]*quat.m_vector[1] + quat.m_scalar*quat.m_vector[2]), 1.0f - 2.0f*(quat.m_vector[0]*quat.m_vector[0] + quat.m_vector[2]*quat.m_vector[2]), 2.0f*(quat.m_vector[1]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[0]), 0.0f}
			, {2.0f*(quat.m_vector[0]*quat.m_vector[2] - quat.m_scalar*quat.m_vector[1]), 2.0f*(quat.m_vector[1]*quat.m_vector[2] + quat.m_scalar*quat.m_vector[0]), 1.0f - 2.0f*(quat.m_vector[0]*quat.m_vector[0] + quat.m_vector[1]*quat.m_vector[1]), 0.0f}
			, {0.0f, 0.0f, 0.0f, 1.0f}
		});
}

Matrix<float, 4, 4> Quaternion::CreateRotationMatrix(const float angle, const Vector<float, 3>& axis)
{
	return CreateRotationMatrix(MakeRotate(angle, axis));
}

Vector<float, 3> Quaternion::rotate(const Quaternion& quat, Vector<float, 3> vector)
{
	// Reference for the optimized implementation:
	//
	//   https://fgiesen.wordpress.com/2019/02/09/rotating-a-single-vector-using-a-quaternion/
	//
	const Vector<float, 3> temp = 2.0f * Cross(quat.m_vector, vector);
	return vector + quat.m_scalar * temp + Cross(quat.m_vector, temp);
}

Vector<float, 3> Quaternion::ToEulerAngles(const Quaternion & quat)
{
	return Vector<float, 3> (
		{
			std::atan2(2.0f*(quat.m_scalar*quat.m_vector[0] + quat.m_vector[1]*quat.m_vector[2]), 1.0f - 2.0f*(quat.m_vector[0]*quat.m_vector[0] + quat.m_vector[1]*quat.m_vector[1])),
			std::asin(2.0f*(quat.m_scalar*quat.m_vector[1] - quat.m_vector[2]*quat.m_vector[0])),
			std::atan2(2.0f*(quat.m_scalar*quat.m_vector[2] + quat.m_vector[0]*quat.m_vector[1]), 1.0f - 2.0f*(quat.m_vector[1]*quat.m_vector[1] + quat.m_vector[2]*quat.m_vector[2]))
		});
}

std::ostream& operator<<(std::ostream& os, const Quaternion& quat)
{
	os << "{ " << quat.m_scalar;
	os << ", " << quat.m_vector[0];
	os << ", " << quat.m_vector[1];
	os << ", " << quat.m_vector[2] << " }";
	return os;
}
