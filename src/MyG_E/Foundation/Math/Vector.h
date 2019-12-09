#pragma once

#include <assimp/vector3.h>

#include "Foundation/\Utility.h"

template<class T, unsigned int NumRow, unsigned int NumCol>
class Matrix;

template<class T, unsigned int M>
class Vector
{
public:
	Vector() = default;

	explicit constexpr Vector(T value)
		:m_vector(std::move(fill_array<T, M>(value)))
	{
	}

	template <class ...Args, typename std::enable_if<(sizeof...(Args) == M), int>::type = 0>
	constexpr Vector(Args ... args)
		:m_vector{args ...}
	{}

	constexpr Vector(std::array<T, M> const& vector)
		: m_vector(vector)
	{}

	constexpr Vector(std::array<T, M>&& vector)
		: m_vector{ std::move(vector) }
	{}
	/* 
	Ambiguity in Vector<int, 2> A({something, something}) becouse the compiler can't choose between constexpr Vector(Args ... args) 
	and constexpr Vector(std::array<T, M>&& vector). Instead, you should call Vector<int, 2> A(something, something) or even
	Vector<int, 2> A{something, something}.
	*/

	// Operators
	T& operator[](unsigned int index)
	{
		return m_vector[index];
	}

	const T& operator[](unsigned int index) const
	{
		return m_vector[index];
	}

	Vector operator*=(T const scalar)
	{
		for (unsigned int i = 0; i < M; i++)
			m_vector[i] *= scalar;
		return *this;
	}

	Vector operator /= (T const scalar)
	{
		for (unsigned int i = 0; i < M; i++)
			m_vector[i] /= scalar;
		return *this;
	}

	Vector operator+=(Vector const& vector2)
	{
		for (unsigned int i = 0; i < M; i++)
			m_vector[i] += vector2.m_vector[i];
		return *this;
	}

	Vector operator-=(Vector const& vector2)
	{
		for (unsigned int i = 0; i < M; i++)
			m_vector[i] -= vector2.m_vector[i];
		return *this;
	}

	Vector operator-()
	{
		Vector3f aux;
		for (unsigned int i = 0; i < M; i++)
			aux.m_vector[i] = -m_vector[i];
		return aux;
	}

	inline bool operator==(Vector const& other) { return std::equal(&m_vector[0], &m_vector[0] + M, other.get_as_pointer()); }

	inline bool operator != (Vector const& other) { return !(*this == other); }

	// Auxiliary function.
	T* get_as_pointer() { return m_vector.data(); }
	T const* get_as_pointer() const { return m_vector.data(); }

	inline float length() const
	{
		float length = 0;
		for (unsigned int i = 0; i < M; i++)
			length += m_vector[i] * m_vector[i];
		return sqrt(length);
	}

	// Return normalizedd vector but dont normalized it.
	inline Vector<T, M> get_normalized() const
	{
		Vector<T, M> aux = *this;
		aux.normalized();
		return aux;
	}

	// normalized vector and return it.
	inline Vector<T, M> normalize() const
	{
		float L = length();
		for (unsigned int i = 0; i < M; i++)
			m_vector[i] = m_vector[i] / L;
		return *this;
	}

	inline bool is_normalized() const
	{
		return length() == 1;
	}

	// dot product
	static T dot(Vector<T, M> const& vector1, Vector<T, M>const& vector2)
	{
		T result = 0;
		for (unsigned int i = 0; i < M; i++)
			result += vector1[i] * vector2[i];
		return result;
	}

private:
	std::array<T, M> m_vector;
};


// Vector 3 specialization
template<class T>
class Vector<T, 3>
{
public:
	Vector() = default;

	explicit constexpr Vector(T value)
		: m_vector{ value, value, value }
	{}

	constexpr Vector(T v1, T v2, T v3)
		: m_vector{v1, v2, v3}
	{}

	constexpr Vector(std::array<T, 3> const& vector)
		: m_vector(vector)
	{}

	constexpr Vector(std::array<T, 3>&& vector)
		: m_vector{ std::move(vector) }
	{}
	/*
	Ambiguity in Vector A({something, something, something}) becouse the compiler can't choose between constexpr Vector(T v1, T v2, T v3)
	and constexpr Vector(std::array<T, M>&& vector). Instead, you should call Vector A(something, something, something) or even
	Vector A{something, something, something}.
	*/

	// assimp vector
	Vector(aiVector3D const& vector)
	{
		*this = vector;
	}

	Vector operator=(aiVector3D const& vector)
	{
		m_vector[0] = vector.x;
		m_vector[1] = vector.y;
		m_vector[2] = vector.z;
		return *this;
	}

	// Operators
	T& operator[](unsigned int index)
	{
		return m_vector[index];
	}

	T const& operator[](unsigned int index) const
	{
		return m_vector[index];
	}

	Vector operator*=(T const scalar)
	{
		m_vector[0] *= scalar;
		m_vector[1] *= scalar;
		m_vector[2] *= scalar;
		return *this;
	}

	Vector operator/=(T const scalar)
	{
		m_vector[0] /= scalar;
		m_vector[1] /= scalar;
		m_vector[2] /= scalar;
		return *this;
	}


	Vector operator+=(Vector const& vector2)
	{
		m_vector[0] += vector2.m_vector[0];
		m_vector[1] += vector2.m_vector[1];
		m_vector[2] += vector2.m_vector[2];
		return *this;
	}

	Vector operator-=(Vector const& vector2)
	{
		m_vector[0] -= vector2.m_vector[0];
		m_vector[1] -= vector2.m_vector[1];
		m_vector[2] -= vector2.m_vector[2];
		return *this;
	}

	Vector operator-()
	{
		Vector3f aux;
		aux.m_vector[0] = -m_vector[0];
		aux.m_vector[1] = -m_vector[1];
		aux.m_vector[2] = -m_vector[2];
		return aux;
	}

	inline bool operator==(Vector const& other) { return std::equal(&m_vector[0], &m_vector[0] + 3, other.get_as_pointer()); }
	inline bool operator != (Vector const& other) { return !(*this == other); }

	// Auxiliary function.
	T* get_as_pointer() { return m_vector.data(); }
	T const* get_as_pointer() const { return m_vector.data(); }

	inline float length() const
	{
		float length = 0;
		length += m_vector[0] * m_vector[0];
		length += m_vector[1] * m_vector[1];
		length += m_vector[2] * m_vector[2];
		return sqrt(length);
	}

	// Return normalizedd vector but dont normalized it.
	inline Vector<T, 3> get_normalized() const
	{
		Vector<T, 3> aux = *this;
		aux.normalize();
		return aux;
	}

	// normalized vector and return it.
	inline Vector<T, 3> normalize()
	{
		float l = length();
		m_vector[0] = m_vector[0] / l;
		m_vector[1] = m_vector[1] / l;
		m_vector[2] = m_vector[2] / l;
		return *this;
	}

	inline bool is_normalized() const { return length() == 1; }

	// dot product
	static T dot(Vector<T, 3> const& vector1, Vector<T, 3> const& vector2)
	{
		T result = 0;
		result += vector1[0] * vector2[0];
		result += vector1[1] * vector2[1];
		result += vector1[2] * vector2[2];
		return result;
	}

	// cross product.
	static Vector<T, 3> cross(Vector<T, 3> const& vector1, Vector<T, 3> const& vector2)
	{
		Vector<T, 3> result;
		result[0] = (vector1[1] * vector2[2] - vector1[2] * vector2[1]);
		result[1] = (vector1[2] * vector2[0] - vector1[0] * vector2[2]);
		result[2] = (vector1[0] * vector2[1] - vector1[1] * vector2[0]);
		return result;
	}

private:
	 std::array<T, 3> m_vector;
};

//cout
template<class T, unsigned int M>
std::ostream& operator<<(std::ostream& os, Vector<T, M> const& vector)
{
	for (unsigned int i = 0; i < M; i++)
	{
		if (i == 0)
			os << "{ " << vector[i];
		else if (i == M - 1)
			os << ", " << vector[i] << " }";
		else
			os << ", " << vector[i];
	}
	return os;
}

template<class V, class T, unsigned int M >
Vector<T, M> operator*(Vector<T, M> vector, V const scalar)
{
	return vector *= scalar;
}

template<class V,  class T, unsigned int M>
Vector<T, M> operator*(V const scalar, Vector<T, M> vector)
{
	return vector *= scalar;
}

template<class V, class T, unsigned int M >
Vector<T, M> operator/(Vector<T, M> vector, V const scalar)
{
	return vector /= scalar;
}

template<class V, class T, unsigned int M>
Vector<T, M> operator/(V const scalar, Vector<T, M> vector)
{
	return vector /= scalar;
}

template<class T, unsigned int M>
Vector<T, M> operator+(Vector<T, M> vector1, Vector<T, M> const& vector2)
{
	return vector1 += vector2;
}

template<class T, unsigned int M>
Vector<T, M> operator-(Vector<T, M> vector1, Vector<T, M> const& vector2)
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

constexpr Vector3f X_AXIS = { 1.0f, 0.0f, 0.0f };
constexpr Vector3f Y_AXIS = { 0.0f, 1.0f, 0.0f };
constexpr Vector3f Z_AXIS = { 0.0f, 0.0f, 1.0f };
