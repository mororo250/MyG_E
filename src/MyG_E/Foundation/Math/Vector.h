#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <memory.h>
#include <math.h>

template<class T, unsigned int NumRow, unsigned int NumCol>
class Matrix;

template<class T, unsigned int NumElem>
class Vector
{
public:
	Vector()
		:mVector{}
	{}


	Vector(T value)
	{
		for (auto& aux : mVector)
			aux = value;
	}

	Vector (T vector[NumElem])
	{
		std::memcpy(mVector, vector, NumElem * sizeof(T));
	}

	Vector (std::initializer_list<T> aux)
	{
		unsigned int i = 0;
		for (auto p : aux)
		{
			mVector[i] = p;
			i++;
		}
	}

	// Operators
	T& operator[](unsigned int index)
	{
		return mVector[index];
	}

	const T& operator[](unsigned int index) const
	{
		return mVector[index];
	}

	Vector& operator*=(const T scalar)
	{
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] *= scalar;
		return *this;
	}

	Vector& operator+=(const Vector& vector2)
	{
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] += vector2.mVector[i];
		return *this;
	}

	Vector& operator-=(const Vector& vector2)
	{
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] -= vector2.mVector[i];
		return *this;
	}

	Vector& operator-()
	{
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] = -mVector[i];
		return *this;
	}

	inline bool operator==(const Vector& other) { return std::equal(mVector, mVector + NumElem, other.mVector); }

	// Auxiliary function.
	auto GetAsPointer() const { return mVector; }
	unsigned int GetNumElem() const { return NumElem; }

	inline float Length()
	{
		float length = 0;
		for (unsigned int i = 0; i < NumElem; i++)
			length += mVector[i] * mVector[i];
		return sqrt(length);
	}

	// Return normalized vector but dont normalize it.
	inline Vector<T, NumElem> GetNormalized()
	{
		Vector<T, NumElem> aux = *this;
		aux.Normalize();
		return aux;
	}

	// Normalize vector and return it.
	inline Vector<T, NumElem> Normalize()
	{
		float L = Length();
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] = mVector[i] / L;
		return *this;
	}

	inline bool isNormalized()
	{
		return Length == 1;
	}

	// Dot product
	static T Dot(const Vector<T, NumElem> vector1, const Vector<T, NumElem> vector2)
	{
		T result = 0;
		for (unsigned int i = 0; i < NumElem; i++)
			result += vector1[i] * vector2[i];
		return result;
	}

	
	template<class U, unsigned int VNumElem, unsigned int MNumCol>
	friend Vector<U, MNumCol> operator*(const Vector<U, VNumElem> vector, const Matrix<U, VNumElem, MNumCol> matrix);

private:
	T mVector[NumElem];
};

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator*(Vector<T, NumElem>& vector, const T scalar)
{
	return vector *= scalar;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator*(const T scalar, Vector<T, NumElem>& vector)
{
	return vector *= scalar;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator+(Vector<T, NumElem>& vector1, Vector<T, NumElem>& vector2)
{
	return vector1 += vector2;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator-(Vector<T, NumElem>& vector1, Vector<T, NumElem>& vector2)
{
	return vector1 -= vector2;
}


template<class T, unsigned int VNumElem, unsigned int MNumCol>
Vector<T, MNumCol> operator*(const Vector<T, VNumElem>& vector, const Matrix<T, VNumElem, MNumCol>& matrix)
{
	Vector<T, MNumCol> Result;
	for (unsigned int i = 0; i < MNumCol; i++)
		for (unsigned int j = 0; j < VNumElem; j++)
			Result.mVector[i] += vector.mVector[j] * matrix.mMatrix[i][j];
	return Result;
}

// Cross product.
Vector<float, 3> Cross(Vector<float, 3> vector1, const Vector<float, 3> vector2);