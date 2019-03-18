#pragma once
/*

#include <iostream>
#include <memory.h>
#include <cmath>

#include "Vector.h"


template<class T, unsigned int NumRow, unsigned int NumCol>
class Matrix
{
public:
	Matrix()
		:mMatrix{}
	{}

	Matrix(T mat[NumRow][NumCol])
	{
		memcpy(mMatrix, mat, NumRow * NumCol * sizeof(T));
	}

	Matrix(std::initializer_list<std::initializer_list<T>> mat)
	{
		unsigned int i = 0;
		for (auto p = mat.begin(); p != mat.end(); ++p)
		{
			unsigned int j = 0;
			for (auto q = (*p).begin(); q != (*p).end(); ++q)
			{
				mMatrix[i][j] = *q;
				j++;
			}
			i++;
		}
	}

	/*need to be improved
	T* operator[](unsigned int index)
	{
		return mMatrix[index];
	}

	const T* operator[](const unsigned int index) const
	{
		return mMatrix[index];
	}
	*/
	/*
	Matrix operator*=(const Matrix RMatrix)
	{
		return *this * RMatrix;
	}

	template<class U>
	Matrix operator*=(const U scalar)
	{
		for (unsigned int i = 0; i < NumRow; i++)
			for (unsigned int j = 0; j < NumCol; j++)
				mMatrix[i][j] *= scalar;
	}

	template<class U, unsigned int LNumRow, unsigned int LNumCol, unsigned int RNumCol>
	friend Matrix<U, LNumRow, RNumCol> operator*(const Matrix<U, LNumRow, LNumCol> &LMatrix, const Matrix<U, LNumCol, RNumCol> &RMatrix);
	template<class U, unsigned int VNumElem, unsigned int MNumCol>
	friend Vector<U, MNumCol> operator*(const Vector<U, VNumElem> vector, const Matrix<U, VNumElem, MNumCol> matrix);

	auto GetAsPointer() const { return mMatrix; }
	T& GetElement(const unsigned int index1, const unsigned int index2) { return mMatrix[index1][index2]; }
	unsigned int GetNumRow() const { return NumRow; }
	unsigned int GetNumCol() const { return NumCol; }

protected:
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

/*
template<class T, unsigned int NumRow, unsigned int NumCol, class S>
Matrix<T, NumRow, NumCol> operator*(Matrix<T, NumRow, NumCol> matrix, S scalar)
{
	return matrix *= scalar;
}

template<class T, unsigned int NumRow, unsigned int NumCol, class S>
Matrix<T, NumRow, NumCol> operator*(S scalar, Matrix<T, NumRow, NumCol> matrix)
{
	return matrix *= scalar;
}
*/

//cout

/*
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
/*
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

class RotationMatrix3 : public Matrix<float, 3, 3>
{
public:
	RotationMatrix3(float angle);
	void SetAngle(const float angle);
};

class RotationMatrix4 : public Matrix<float, 4, 4>
{
public:
	RotationMatrix4(float angle);
	void SetAngle(const float angle);
};

//Create Matrix

Matrix<float, 3, 3> CreateOrthoMatrix(const float left, const float right, const float up, const float down);
*/