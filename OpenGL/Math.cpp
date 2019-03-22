#include "Math.h"

Vector<float, 3> Cross(Vector<float, 3> vector1, const Vector<float, 3> vector2)
{
	Vector<float, 3> result;
	result[0] = (vector1[1] * vector2[2] - vector1[2] * vector2[1]);
	result[1] = (vector1[2] * vector2[0] - vector1[0] * vector2[2]);
	result[2] = (vector1[0] * vector2[1] - vector1[1] * vector2[0]);
	return result;
}

Matrix<float, 4, 4> LookAt(const Vector<float, 3> cameraPosition, const Vector<float, 3> targetPosition, const Vector<float, 3> up)
{
	Vector<float, 3> Zaxis = targetPosition - cameraPosition;
	Zaxis.Normalize();
	Vector<float, 3> Xaxis = Cross(up, Zaxis);
	Xaxis.Normalize();
	Vector<float, 3> Yaxis = (Cross(Zaxis, Xaxis));
	Yaxis.Normalize();
	return Matrix<float, 4, 4>{ {Xaxis[0], Yaxis[0], Zaxis[0], 0},
	{ Xaxis[1], Yaxis[1], Zaxis[1], 0 },
	{ Xaxis[2], Yaxis[2], Zaxis[2], 0 },
	{ -Dot(Xaxis, cameraPosition), -Dot(Yaxis, cameraPosition), -Dot(Zaxis, cameraPosition), 1 }};
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

Matrix<float, 4, 4> CreatePerspectiveMatrix(const float left, const float right, const float top, const float botton, const float Near, const float far)
{
	Matrix<float, 4, 4> PerspectiveMatrix{ { 2 * Near / (right - left), 0, 0, 0},
											 { 0, 2 * Near / (top - botton), 0, 0},
											 { (right + left) / (right - left), (top + botton) / (top - botton), -(far + Near) / (far - Near), -1},
											 { 0, 0, -(2 * far * Near) / (far - Near), 0} };
	return PerspectiveMatrix;
}

TranslationMatrix3::TranslationMatrix3(const float TranX, const float TranY)
	:Matrix<float, 3, 3>{ {1 , 0, 0 }, {0, 1, 0}, {TranX, TranY, 1} }
{}


TranslationMatrix4::TranslationMatrix4(const float xTrans, const float yTrans, const float zTrans)
	: Matrix<float, 4, 4>{ {1 , 0, 0, 0 }, {0, 1, 0, 0}, {0, 0, 1, 0}, {xTrans, yTrans, zTrans, 1} }
{}

ScaleMatrix3::ScaleMatrix3(const float ScaleX, const float ScaleY)
	: Matrix<float, 3, 3>{ {ScaleX, 0, 0 }, {0, ScaleY, 0}, {0, 0, 1} }
{}

ScaleMatrix4::ScaleMatrix4(const float ScaleX, const float ScaleY, const float ScaleZ)
	: Matrix<float, 4, 4>{ { {ScaleX , 0, 0, 0 }, {0, ScaleY, 0, 0}, {0, 0, ScaleZ, 0}, {0, 0, 0, 1} } }
{}

RotationMatrix3::RotationMatrix3(float angle)
	: Matrix<float, 3, 3>{ {cos(angle), sin(angle),  0 }, {-sin(angle), cos(angle), 0}, {0, 0, 1} }
{}


void RotationMatrix3::SetAngle(const float angle)
{
	GetElement(0, 0) = cos(angle);
	GetElement(0, 1) = sin(angle);
	GetElement(1, 1) = cos(angle);
	GetElement(1, 0) = -sin(angle);
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
		GetElement(1, 1) = cos(angle);
		GetElement(1, 2) = sin(angle);
		GetElement(2, 1) = -sin(angle);
		GetElement(2, 2) = cos(angle);
		break;

	case AxisUsage::AXIS_Y:
		GetElement(0, 0) = cos(angle);
		GetElement(0, 2) = -sin(angle);
		GetElement(2, 0) = sin(angle);
		GetElement(2, 2) = cos(angle);
		break;

	case AxisUsage::AXIS_Z:
		GetElement(0, 0) = cos(angle);
		GetElement(0, 1) = sin(angle);
		GetElement(1, 1) = cos(angle);
		GetElement(1, 0) = -sin(angle);
		break;

	}
}

EulerAngles::EulerAngles()
	:mRotX(0.0f, AxisUsage::AXIS_X),
	mRotY(0.0f, AxisUsage::AXIS_Y),
	mRotZ(0.0f, AxisUsage::AXIS_Z)
{
}

void EulerAngles::RotateXYZ(float angleX, float angleY, float angleZ)
{
	mRotX.SetAngle(angleX);
	mRotY.SetAngle(angleY);
	mRotZ.SetAngle(angleZ);
}
