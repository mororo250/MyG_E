#pragma once

#include <iostream>
#include <memory.h>
#include <cmath>

#include "Foundation\Math\Vector.h"

template<class T, unsigned int NumRow, unsigned int NumCol>
class Matrix
{
public:
	Matrix()
		:mMatrix{}
	{}

	Matrix(T mat[NumRow][NumCol])
	{
		std::memcpy(mMatrix, mat, NumRow * NumCol * sizeof(T));
	}

	Matrix(std::initializer_list<std::initializer_list<T>> mat)
	{
		unsigned int i = 0;
		for (const auto& aux1 : mat)
		{
			unsigned int j = 0;
			for (const auto& aux2 : aux1)
			{
				mMatrix[i][j] = aux2;
				j++;
			}
			i++;
		}
	}

	Matrix operator*=(const Matrix RMatrix)
	{
		return *this * RMatrix;
	}

	Matrix operator*=(const T scalar)
	{
		for (unsigned int i = 0; i < NumRow; i++)
			for (unsigned int j = 0; j < NumCol; j++)
				mMatrix[i][j] *= scalar;
		return *this;
	}

	template<class U, unsigned int LNumRow, unsigned int LNumCol, unsigned int RNumCol>
	friend Matrix<U, LNumRow, RNumCol> operator*(const Matrix<U, LNumRow, LNumCol> &LMatrix, const Matrix<U, LNumCol, RNumCol> &RMatrix);
	template<class T, unsigned int VNumElem, unsigned int MNumCol>
	friend Vector<T, MNumCol> operator*(const Vector<T, VNumElem>& vector, const Matrix<T, VNumElem, MNumCol>& matrix);

	auto GetAsPointer() const { return mMatrix; }
	T& GetElement(const unsigned int index1, const unsigned int index2) { return mMatrix[index1][index2]; }

	unsigned int GetNumRow() const { return NumRow; }
	unsigned int GetNumCol() const { return NumCol; }

	Matrix<T, NumRow, NumCol> Transpose()
	{
		for (unsigned int i = 0; i < NumRow; i++)
			for (unsigned int j = 0; j < NumCol; j++)
				mMatrix[i][j] = mMatrix[j][i];
		return *this;
	}

private:
	T mMatrix[NumRow][NumCol];
};

template<class T, unsigned int LNumRow, unsigned int LNumCol, unsigned int RNumCol>
Matrix<T, LNumRow, RNumCol> operator*(const Matrix<T, LNumRow, LNumCol> &LMatrix, const Matrix<T, LNumCol, RNumCol> &RMatrix)
{
	Matrix<T, LNumRow, RNumCol> Result;
	for (unsigned int i = 0; i < LNumRow; i++)
		for (unsigned int j = 0; j < RNumCol; j++)
			for (unsigned int x = 0; x < LNumCol; x++)
				Result.mMatrix[i][j] += LMatrix.mMatrix[i][x] * RMatrix.mMatrix[x][j];
	return Result;
}

//sclar mutiplication

template<class T, unsigned int NumRow, unsigned int NumCol>
Matrix<T, NumRow, NumCol> operator*(Matrix<T, NumRow, NumCol> matrix, T scalar)
{
	return matrix *= scalar;
}

template<class T, unsigned int NumRow, unsigned int NumCol>
Matrix<T, NumRow, NumCol> operator*(T scalar, Matrix<T, NumRow, NumCol> matrix)
{
	return matrix *= scalar;
}

// Vector and Matrix mutiplication

template<class T, unsigned int VNumElem, unsigned int MNumCol>
Vector<T, MNumCol> operator*(const Vector<T, VNumElem>& vector, const Matrix<T, VNumElem, MNumCol>& matrix)
{
	Vector<T, MNumCol> Result;
	for (unsigned int i = 0; i < MNumCol; i++)
		for (unsigned int j = 0; j < VNumElem; j++)
			Result[i] += vector[j] * matrix.mMatrix[i][j];
		return Result;
}

//cout
template<class T, unsigned int NumRow, unsigned int NumCol>
std::ostream& operator<<(std::ostream& os, const Matrix<T, NumRow, NumCol>& matrix)
{
	unsigned int aux = NumCol - 1;
	for (unsigned int i = 0; i < NumRow; i++)
		for (unsigned int j = 0; j < NumCol; j++)
			if (j == 0)
				os << "{\t" << matrix.GetElement(i, j);
			else if (j == aux)
				os << ",\t" << matrix.GetElement(i, j) << "\t}\n";
			else
				os << ",\t" << matrix.GetElement(i, j);
	return os;
}


//specific Matrix

class TranslationMatrix3 : public Matrix<float, 3, 3>
{
public:
	TranslationMatrix3(const float TranX, const float TranY);
	const float GetTranX() { return GetElement(2, 0); }
	const float GetTranY() { return GetElement(2, 1); }
	void SetTranX(float TranX) { GetElement(2, 0) = TranX; }
	void SetTranY(float TranY) { GetElement(2, 1) = TranY; }
};

class TranslationMatrix4 : public Matrix<float, 4, 4>
{
public:
	TranslationMatrix4(const float TranX, const float TranY, const float TranZ);
	const float GetTranX() { return GetElement(3, 0); }
	const float GetTranY() { return GetElement(3, 1); }
	const float GetTranZ() { return GetElement(3, 2); }
	void SetTranX(const float TranX) { GetElement(3, 0) = TranX; }
	void SetTranY(const float TranY) { GetElement(3, 1) = TranY; }
	void SetTranZ(const float TranZ) { GetElement(3, 2) = TranZ; }
};

class ScaleMatrix3 : public Matrix<float, 3, 3>
{
public:
	ScaleMatrix3(const float ScaleX, const float ScaleY);
	const float GetScaleX() { return GetElement(0, 0); }
	const float GetScaleY() { return GetElement(1, 1); }
	void SetScaleX(const float &ScaleX) { GetElement(0, 0) = ScaleX; }
	void SetScaleY(const float &ScaleY) { GetElement(1, 1) = ScaleY; }
};

class ScaleMatrix4 : public Matrix<float, 4, 4>
{
public:
	ScaleMatrix4(const float ScaleX, const float ScaleY, const float ScaleZ);
	const float GetScaleX() { return GetElement(0, 0); }
	const float GetSclaeY() { return GetElement(1, 1); }
	const float GetScaleZ() { return GetElement(2, 2); }
	void SetScaleX(const float ScaleX) { GetElement(0, 0) = ScaleX; }
	void SetScaleY(const float ScaleY) { GetElement(1, 1) = ScaleY; }
	void SetScaleZ(const float ScaleZ) { GetElement(2, 2) = ScaleZ; }
};

enum class AxisUsage
{
	AXIS_X, AXIS_Y, AXIS_Z
};

class RotationMatrix3 : public Matrix<float, 3, 3>
{
public:
	RotationMatrix3(float angle, AxisUsage axis = AxisUsage::AXIS_Z);
	void SetAngle(const float angle);

private:
	AxisUsage mAxis;
};

class RotationMatrix4 : public Matrix<float, 4, 4>
{
public:
	RotationMatrix4(float angle, AxisUsage axis = AxisUsage::AXIS_Z);
	void SetAngle(const float angle);
private:
	AxisUsage mAxis;
};

class EulerAngles
{
public:
	EulerAngles(float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f);

	inline void RotateX(float angle) { m_rotX.SetAngle(angle); }
	inline void RotateY(float angle) { m_rotY.SetAngle(angle); }
	inline void RotateZ(float angle) { m_rotZ.SetAngle(angle); }
	void RotateXYZ(float angleX, float angleY, float angleZ);

	inline Matrix<float, 4, 4> GetRotation() { return m_rotZ * m_rotY * m_rotX; }
	inline float GetYaw() { return m_yaw; }
	inline float GetPitch() { return m_pitch; }
	inline float GetRoll() { return m_roll; }

private:
	// Angles.
	float m_yaw;
	float m_pitch;
	float m_roll;

	// Matrices.
	RotationMatrix4 m_rotX;
	RotationMatrix4 m_rotY;
	RotationMatrix4 m_rotZ;
};

// Fuctions that create a matrix.
inline float ToRadiants(float angle);

Matrix<float, 3, 3> CreateOrthoMatrix(const float left, const float right, const float top, const float bottom);
Matrix<float, 4, 4> LookAt(const Vector<float, 3> cameraPosition, const Vector<float, 3> targetPosition, const Vector<float, 3>up);
Matrix<float, 4, 4> CreateOrthographicMatrix(const float left, const float right, const float top, const float botton, const float _near, const float _far);
Matrix<float, 4, 4> CreatePerspectiveMatrix(const float fov, const float aspectRatio, const float Near, const float far);