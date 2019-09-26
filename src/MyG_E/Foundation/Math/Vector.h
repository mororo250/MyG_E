#pragma once

#define _USE_MATH_DEFINES // Math constants

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

	Vector operator-()
	{
		Vector<float, 3> aux;
		for (unsigned int i = 0; i < NumElem; i++)
			aux.mVector[i] = -mVector[i];
		return aux;
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
		return Length() == 1;
	}

	// Dot product
	static T Dot(const Vector<T, NumElem> vector1, const Vector<T, NumElem> vector2)
	{
		T result = 0;
		for (unsigned int i = 0; i < NumElem; i++)
			result += vector1[i] * vector2[i];
		return result;
	}

private:
	T mVector[NumElem];
};

//cout
template<class T, unsigned int NumElem>
std::ostream& operator<<(std::ostream& os, const Vector<T, NumElem>& vector)
{
	for (unsigned int i = 0; i < NumElem; i++)
	{
		if (i == 0)
			os << "{ " << vector[i];
		else if (i == NumElem - 1)
			os << ", " << vector[i] << " }";
		else
			os << ", " << vector[i];
	}
	return os;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator*(Vector<T, NumElem> vector, const T scalar)
{
	return vector *= scalar;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator*(const T scalar, Vector<T, NumElem> vector)
{
	return vector *= scalar;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator+(Vector<T, NumElem> vector1, const Vector<T, NumElem>& vector2)
{
	return vector1 += vector2;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator-(Vector<T, NumElem> vector1, const Vector<T, NumElem>& vector2)
{
	return vector1 -= vector2;
}

// Cross product.
Vector<float, 3> Cross(const Vector<float, 3>& vector1, const Vector<float, 3>& vector2);