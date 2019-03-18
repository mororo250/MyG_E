#include "Matrix.h"

/*
Matrix<float, 3, 3> CreateOrthoMatrix(const float left, const float right, const float up, const float down)
{
	Matrix<float, 3, 3> OrthoMatrix{ { 2.0f / (right + left) , 0.0f , 0.0f},
									 { 0.0f , 2.0f / (up + down) , 0.0f},
									 { -(right - left) / (right + left), -(up - down) / (up + down), 1.0f} };
	return OrthoMatrix;
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


Matrix<float, 4, 4> CreateRotationMatrix4(float angle)
{
	Matrix<float, 4, 4> Rmatrix{ {1 , 0, 0, 0 }, {0, cos(angle), -sin(angle), 0}, {0, sin(angle), cos(angle), 0}, {0, 0, 0, 1} };
	return Rmatrix;
}

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

RotationMatrix4::RotationMatrix4(float angle)
	: Matrix<float, 4, 4>{ {1 , 0, 0, 0 }, {0, cos(angle), -sin(angle), 0}, {0, sin(angle), cos(angle), 0}, {0, 0, 0, 1} }
{}

void RotationMatrix4::SetAngle(const float angle)
{
	GetElement(1, 1) = cos(angle);
	GetElement(2, 1) = sin(angle);
	GetElement(2, 2) = cos(angle);
	GetElement(1, 2) = -sin(angle);
}
*/