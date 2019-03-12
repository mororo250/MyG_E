#pragma once

#include <iostream>
#include <memory.h>
#include <cmath>

template<class T, unsigned int NumRow, unsigned int NumCol>
class Matrix;

template<class T, unsigned int NumElem>
class Vector
{
public:
	Vector()
	:mVector{}
	{}
	

	Vector(T vector[NumElem])
	{
		memcpy(mVector, vector, NumElem * sizeof(T));
	}

	Vector(std::initializer_list<T> mat)
	{
		unsigned int i = 0;
		for (auto p = mat.begin(); p != mat.end(); ++p)
		{
			mVector[i] = *p;
			i++;
		}
	}


	T* operator[](unsigned int index)
	{
		return mVector[index];
	}

	const T* operator[](unsigned int index) const
	{
		return mVector[index];
	}

	template<class U>
	Vector operator*=(const U scalar)
	{
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] *= scalar;
	}

	template<class U, unsigned int VNumElem, unsigned int MNumCol>
	friend Vector<U, MNumCol> operator*(const Vector<U, VNumElem> vector, const Matrix<U, VNumElem, MNumCol> matrix);

	auto GetAsPointer() const { return mVector; }
	unsigned int GetNumElem() const { return NumElem; }

	float length()
	{
		float length = 0;
		for (unsigned int i = 0; i < NumElem; i++)
			length += mVector[i] * mVector[i];
		return sqrt(length);
	}

	void normalize()
	{
		float L = length();
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] = mVector[i] / L;
	}

private:
	T mVector[NumElem];
};

template<class T, unsigned int NumElem, class S>
Vector<T, NumElem> operator*(const Vector<T, NumElem> vector, const S scalar)
{
	return vector *= scalar;
}

template<class T, unsigned int NumElem, class S>
Vector<T, NumElem> operator*(const S scalar, const Vector<T, NumElem> vector)
{
	return vector *= scalar;
}

template<class T, unsigned int VNumElem, unsigned int MNumCol>
Vector<T, MNumCol> operator*(const Vector<T, VNumElem> vector, const Matrix<T, VNumElem, MNumCol> matrix)
{
	Vector<T, MNumCol> Result;
	for (unsigned int i = 0; i < MNumCol; i++)
		for (unsigned int j = 0; j < VNumElem; j++)
			Result.mVector[i] += vector.mVector[j] * matrix.mMatrix[i][j];
	return Result;
}

template<class T, unsigned int NumElem>
std::ostream& operator<<(std::ostream& os, const Vector<T, NumElem>& vector)
{
	unsigned int aux = NumElem - 1;
	for (unsigned int i = 0; i < NumElem; i++)
		if (i == 0)
			os << "{\t" << vector[i];
		else if (i == aux)
			os << ",\t" << vector[i] << "\t}";
		else
			os << ",\t" << vector[i];
	return os;
}


//matrix


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

	T* operator[](unsigned int index)
	{
		return mMatrix[index];
	}

	const T* operator[](const unsigned int index) const
	{
		return mMatrix[index];
	}

	Matrix operator*=(const Matrix RMatrix)
	{
		return this* * RMatrix;
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
	unsigned int GetNumRow() const { return NumRow; }
	unsigned int GetNumCol() const { return NumCol; }

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
template<class T, unsigned int NumRow, unsigned int NumCol, class S>
Matrix<T, NumRow, NumCol> operator*(const Matrix<T, NumRow, NumCol> matrix, const S scalar)
{
	return matrix *= scalar;
}
template<class T, unsigned int NumRow, unsigned int NumCol, class S>
Matrix<T, NumRow, NumCol> operator*(const S scalar, const Matrix<T, NumRow, NumCol> matrix)
{
	return matrix *= scalar;
}

//cout
template<class T, unsigned int NumRow, unsigned int NumCol>
std::ostream& operator<<(std::ostream& os, const Matrix<T, NumRow, NumCol>& matrix)
{
	unsigned int aux = NumCol - 1;
	for (unsigned int i = 0; i < NumRow; i++)
		for (unsigned int j = 0; j < NumCol; j++)
			if (j == 0)
				os << "{\t" << matrix[i][j];
			else if (j == aux)
				os << ",\t" << matrix[i][j] << "\t}\n";
			else
				os << ",\t" << matrix[i][j];
	return os;
}


//specific Matrix

class TranslationMatrix : public Matrix<float, 3, 3>
{
public:
	Matrix<float, 3, 3> CreateTranslationMatrix3(float xTrans, float yTrans);
	float GetTranX() const { return mTranX; }

private: 
	float mTranX;
	float mTranY;
};

//Create Matrix

Matrix<float, 3, 3> CreateOrthoMatrix(const float left, const float right, const float up, const float down);
Matrix<float, 3, 3> CreateTranslationMatrix3(float xTrans, float yTrans);
Matrix<float, 4, 4> CreateTranslationMatrix4(float xTrans, float yTrans, float zTrans);
Matrix<float, 3, 3> CreateScaleMatrix3(float xScale, float yScale);
Matrix<float, 4, 4> CreateScaleMatrix4(float xScale, float yScale, float zScale);
Matrix<float, 3, 3> CreateRotationMatrix3(float angle);
Matrix<float, 4, 4> CreateRotationMatrix4(float angle);