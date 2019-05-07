#include "Foundation/Math/Matrix.h"

Matrix<float, 4, 4> LookAt(const Vector<float, 3> cameraPosition, const Vector<float, 3> targetPosition, const Vector<float, 3> up)
{
	Vector<float, 3> Zaxis = cameraPosition - targetPosition;
	Zaxis.Normalize();
	Vector<float, 3> Xaxis = Cross(up, Zaxis);
	Xaxis.Normalize();
	Vector<float, 3> Yaxis = (Cross(Zaxis, Xaxis));
	Yaxis.Normalize();
	return Matrix<float, 4, 4>{ {Xaxis[0], Yaxis[0], Zaxis[0], 0},
	{ Xaxis[1], Yaxis[1], Zaxis[1], 0 },
	{ Xaxis[2], Yaxis[2], Zaxis[2], 0 },
	{ -Vector<float, 3>::Dot(Xaxis, cameraPosition), -Vector<float, 3>::Dot(Yaxis, cameraPosition), -Vector<float, 3>::Dot(Zaxis, cameraPosition), 1 }};
}


inline float ToRadiants(float angle)
{
	return angle * 3.14 / 180;
}

Matrix<float, 3, 3> CreateOrthoMatrix(const float left, const float right, const float up, const float down)
{
	Matrix<float, 3, 3> OrthoMatrix{ { 2.0f / (right + left) , 0.0f , 0.0f},
									 { 0.0f , 2.0f / (up + down) , 0.0f},
									 { -(right - left) / (right + left), -(up - down) / (up + down), 1.0f} };
	return OrthoMatrix;
}

Matrix<float, 4, 4> CreateOrthographicMatrix(const float left, const float right, const float top, const float botton, const float _near, const float _far)
{
	Matrix<float, 4, 4> OrthoMatrix{{ 2.0f / (right + left) , 0.0f , 0.0f, 0.0f},
									{ 0.0f , 2.0f / (top + botton) , 0.0f, 0.0f},
									{ 0.0, 0.0, -2.0f / (_far - _near), 0.0f},
									{-(right - left) / (right + left), -(top - botton) / (top + botton), -(_far + _near) / (_far - _near), 1.0f} };
	return OrthoMatrix;
}

Matrix<float, 4, 4> CreatePerspectiveMatrix(const float fov, const float aspectRatio, const float Near, const float far)
{
	float fovr = ToRadiants(fov);
	Matrix<float, 4, 4> PerspectiveMatrix{{ 1.0f / (aspectRatio * (tan(fovr/2))), 0.0f, 0.0f, 0.0f},
											{ 0, 1.0f / tan(fovr/2), 0, 0},
											{ 0.0f, 0.0f, -(far + Near) / (far - Near), -1.0f},
											{ 0, 0, -(2.0f * far * Near) / (far - Near), 0.0f} };
	return PerspectiveMatrix;
}

TranslationMatrix3::TranslationMatrix3(const float TranX, const float TranY)
	:Matrix<float, 3, 3>{ {1 , 0, 0 }, {0, 1, 0}, {TranX, TranY, 1} }
{
}


TranslationMatrix4::TranslationMatrix4(const float xTrans, const float yTrans, const float zTrans)
	: Matrix<float, 4, 4>{ {1 , 0, 0, 0 }, {0, 1, 0, 0}, {0, 0, 1, 0}, {xTrans, yTrans, zTrans, 1} }
{
}

ScaleMatrix3::ScaleMatrix3(const float ScaleX, const float ScaleY)
	: Matrix<float, 3, 3>{ {ScaleX, 0, 0 }, {0, ScaleY, 0}, {0, 0, 1} }
{
}

ScaleMatrix4::ScaleMatrix4(const float ScaleX, const float ScaleY, const float ScaleZ)
	: Matrix<float, 4, 4>{ { {ScaleX , 0, 0, 0 }, {0, ScaleY, 0, 0}, {0, 0, ScaleZ, 0}, {0, 0, 0, 1} } }
{
}

RotationMatrix3::RotationMatrix3(float angle, AxisUsage axis)
	: Matrix<float, 3, 3>{ {1, 0, 0}, { 0, 1, 0}, { 0, 0, 1} },
	mAxis(axis)
{
	SetAngle(angle);
}


void RotationMatrix3::SetAngle(const float angle)
{
	switch (mAxis)
	{
	case AxisUsage::AXIS_X:
		GetElement(1, 1) = std::cos(angle);
		GetElement(1, 2) = std::sin(angle);
		GetElement(2, 1) = -std::sin(angle);
		GetElement(2, 2) = std::cos(angle);
		break;

	case AxisUsage::AXIS_Y:
		GetElement(0, 0) = std::cos(angle);
		GetElement(0, 2) = -std::sin(angle);
		GetElement(2, 0) = std::sin(angle);
		GetElement(2, 2) = std::cos(angle);
		break;

	case AxisUsage::AXIS_Z:
		GetElement(0, 0) = std::cos(angle);
		GetElement(0, 1) = std::sin(angle);
		GetElement(1, 1) = std::cos(angle);
		GetElement(1, 0) = -std::sin(angle);
		break;

	}
}


RotationMatrix4::RotationMatrix4(float angle, AxisUsage axis)
	:Matrix<float, 4, 4>{ {1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } },
	mAxis(axis)
{
	SetAngle(angle);
}

void RotationMatrix4::SetAngle(const float angle)
{
	switch (mAxis)
	{
	case AxisUsage::AXIS_X:
		GetElement(1, 1) = std::cos(angle);
		GetElement(1, 2) = std::sin(angle);
		GetElement(2, 1) = -std::sin(angle);
		GetElement(2, 2) = std::cos(angle);
		break;

	case AxisUsage::AXIS_Y:
		GetElement(0, 0) = std::cos(angle);
		GetElement(0, 2) = -std::sin(angle);
		GetElement(2, 0) = std::sin(angle);
		GetElement(2, 2) = std::cos(angle);
		break;

	case AxisUsage::AXIS_Z:
		GetElement(0, 0) = std::cos(angle);
		GetElement(0, 1) = std::sin(angle);
		GetElement(1, 1) = std::cos(angle);
		GetElement(1, 0) = -std::sin(angle);
		break;

	}
}

EulerAngles::EulerAngles(float yaw, float pitch, float roll)
	: m_yaw(yaw)
	, m_pitch(pitch)
	, m_roll(roll)
	, m_rotX(m_yaw, AxisUsage::AXIS_X)
	, m_rotY(m_pitch, AxisUsage::AXIS_Y)
	, m_rotZ(m_roll, AxisUsage::AXIS_Z)
{
}

void EulerAngles::RotateXYZ(float angleX, float angleY, float angleZ)
{
	m_yaw = angleX;
	m_pitch = angleY;
	m_roll = angleZ;
	m_rotX.SetAngle(m_yaw);
	m_rotY.SetAngle(m_pitch);
	m_rotZ.SetAngle(m_roll);
}
