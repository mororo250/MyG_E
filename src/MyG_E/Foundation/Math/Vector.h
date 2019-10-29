#pragma once

#include <assimp/vector3.h>

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
		Vector3f aux;
		for (unsigned int i = 0; i < NumElem; i++)
			aux.mVector[i] = -mVector[i];
		return aux;
	}

	inline bool operator==(const Vector& other) { return std::equal(mVector, mVector + NumElem, other.mVector); }

	// Auxiliary function.
	auto GetAsPointer() const { return mVector; }
	unsigned int GetNumElem() const { return NumElem; }

	inline float Length() const
	{
		float length = 0;
		for (unsigned int i = 0; i < NumElem; i++)
			length += std::fabsf(mVector[i] * mVector[i]);
		return sqrt(length);
	}

	// Return normalized vector but dont normalize it.
	inline Vector<T, NumElem> GetNormalized() const
	{
		Vector<T, NumElem> aux = *this;
		aux.Normalize();
		return aux;
	}

	// Normalize vector and return it.
	inline Vector<T, NumElem> Normalize() const
	{
		float L = Length();
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] = mVector[i] / L;
		return *this;
	}

	inline bool isNormalized() const
	{
		return Length() == 1;
	}

	// Dot product
	static T Dot(Vector<T, NumElem> const& vector1, Vector<T, NumElem>const& vector2)
	{
		T result = 0;
		for (unsigned int i = 0; i < NumElem; i++)
			result += vector1[i] * vector2[i];
		return result;
	}

private:
	T mVector[NumElem];
};


// Vector 3 specialization
template<class T>
class Vector<T, 3>
{
public:
	constexpr Vector()
		:mVector{}
	{}

	constexpr Vector(T value)
		: mVector{ value, value, value }
	{}

	constexpr Vector(T vector[3])
		: mVector{ vector[0], vector[1], vector[2] }
	{}

	constexpr Vector(T v1, T v2, T v3)
		: mVector{v1, v2, v3}
	{}

	 Vector(std::initializer_list<T> aux)
	{
		unsigned int i = 0;
		for (auto p : aux)
		{
			mVector[i] = p;
			i++;
		}
	}

	// assimp vector
	Vector(aiVector3D const& vector)
	{
		*this = vector;
	}

	Vector& operator=(aiVector3D const& vector)
	{
		mVector[0] = vector.x;
		mVector[1] = vector.y;
		mVector[2] = vector.z;
		return *this;
	}

	// Operators
	T& operator[](unsigned int index)
	{
		return mVector[index];
	}



	T const& operator[](unsigned int index) const
	{
		return mVector[index];
	}

	template<class V>
	Vector& operator*=(const V scalar)
	{
		mVector[0] *= scalar;
		mVector[1] *= scalar;
		mVector[2] *= scalar;
		return *this;
	}

	Vector& operator+=(const Vector& vector2)
	{
		mVector[0] += vector2.mVector[0];
		mVector[1] += vector2.mVector[1];
		mVector[2] += vector2.mVector[2];
		return *this;
	}

	Vector& operator-=(const Vector& vector2)
	{
		mVector[0] -= vector2.mVector[0];
		mVector[1] -= vector2.mVector[1];
		mVector[2] -= vector2.mVector[2];
		return *this;
	}

	Vector operator-()
	{
		Vector3f aux;
		aux.mVector[0] = -mVector[0];
		aux.mVector[1] = -mVector[1];
		aux.mVector[2] = -mVector[2];
		return aux;
	}

	inline bool operator==(const Vector& other) { return std::equal(mVector, mVector + 3, other.mVector); }

	// Auxiliary function.
	auto GetAsPointer() const { return mVector; }
	unsigned int GetNumElem() const { return 3; }

	inline float Length() const
	{
		float length = 0;
		for (unsigned int i = 0; i < 3; i++)
			length += std::fabsf(mVector[i] * mVector[i]);
		return sqrt(length);
	}

	// Return normalized vector but dont normalize it.
	inline Vector<T, 3> GetNormalized() const
	{
		Vector<T, 3> aux = *this;
		aux.Normalize();
		return aux;
	}

	// Normalize vector and return it.
	inline Vector<T, 3> Normalize()
	{
		float L = Length();
		for (unsigned int i = 0; i < 3; i++)
			mVector[i] = mVector[i] / L;
		return *this;
	}

	inline bool isNormalized() const
	{
		return Length() == 1;
	}

	// Dot product
	static T Dot(Vector<T, 3> const& vector1, Vector<T, 3> const& vector2)
	{
		T result = 0;
		for (unsigned int i = 0; i < 3; i++)
			result += vector1[i] * vector2[i];
		return result;
	}

	// Cross product.
	static Vector<T, 3> Cross(Vector<T, 3> const& vector1, Vector<T, 3> const& vector2)
	{
		Vector<T, 3> result;
		result[0] = (vector1[1] * vector2[2] - vector1[2] * vector2[1]);
		result[1] = (vector1[2] * vector2[0] - vector1[0] * vector2[2]);
		result[2] = (vector1[0] * vector2[1] - vector1[1] * vector2[0]);
		return result;
	}

private:
	 T mVector[3];
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

template<class V, class T, unsigned int NumElem >
Vector<T, NumElem> operator*(Vector<T, NumElem> vector, const V scalar)
{
	return vector *= scalar;
}

template<class V,  class T, unsigned int NumElem>
Vector<T, NumElem> operator*(const V scalar, Vector<T, NumElem> vector)
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

typedef Vector<int, 4> Vector4i;
typedef Vector<float, 4> Vector4f;
typedef Vector<double, 4> Vector4d;
typedef Vector<int, 3> Vector3i;
typedef Vector<float, 3> Vector3f;
typedef Vector<double, 3> Vector3d;
typedef Vector<int, 2> Vector2i;
typedef Vector<float, 2> Vector2f;
typedef Vector<double, 2> Vector2d;

static constexpr Vector3f X_AXIS = Vector3f(1.0f, 0.0f, 0.0f);
static constexpr Vector3f Y_AXIS = Vector3f(0.0f, 1.0f, 0.0f);
static constexpr Vector3f Z_AXIS = Vector3f(0.0f, 0.0f, 1.0f);
