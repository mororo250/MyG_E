#pragma once

/*

#include "Matrix.h"

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


	T operator[](unsigned int index)
	{
		return mVector[index];
	}

	const T operator[](unsigned int index) const
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
*/