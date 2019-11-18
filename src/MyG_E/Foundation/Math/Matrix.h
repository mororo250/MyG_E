#pragma once

#include "Foundation\Math\Vector.h"
#include "ExtraMathConstant.h"

template<class T, unsigned int M, unsigned int N>
class Matrix
{
public:
	Matrix() = default;

	template <class ...Args, typename std::enable_if<(sizeof...(Args) == (M * N)), int>::type = 0>
	constexpr Matrix(Args ... args)
		:m_matrix{ args ... }
	{}

	constexpr Matrix(std::array<T, N * M> const& mat)
		:m_matrix{ mat }
	{}

	constexpr Matrix(std::array<T, N * M>&& mat)
		:m_matrix{ std::move(mat) }
	{}
	/*
	Ambiguity in Matrix<T,M, N> A({something, something, ...}) becouse the compiler can't choose between constexpr Matrix(Args ... args)
	and constexpr  Matrix(std::array<T, M * N>&& mat). Instead, you should call Matrix<T,M, N> A(something, something, ...) or even
	Matrix<T,M, N> A{something, something, ...}.
	*/

	inline T& operator()(unsigned int const row, unsigned int const col) { return m_matrix[row * 4 + col]; }
	inline T const& operator()(unsigned int const row, unsigned int const col) const { return m_matrix[row * 4 + col]; } 

	Matrix& operator*=(Matrix const& Rmatrix)
	{
		static_assert((N == M), "requires quadratic matrix");
		Matrix aux = *this;
		for (unsigned int i = 0; i < M; i++)
			for (unsigned int j = 0; j < N; j++)
				for (unsigned int x = 0; x < N; x++)
					m_matrix[i * N + j] += aux.m_matrix[i * N + x] * Rmatrix.m_matrix[x * (i * N) + j];
		return *this;
	}

	Matrix& operator*=(T const scalar)
	{
		for (unsigned int i = 0; i < M; i++)
			for (unsigned int j = 0; j < N; j++)
				m_matrix[i * N + j] *= scalar;
		return *this;
	}

	Matrix& operator/=(T const scalar)
	{
		for (unsigned int i = 0; i < M; i++)
			for (unsigned int j = 0; j < N; j++)
				m_matrix[i * N + j] /= scalar;
		return *this;
	}

	Matrix& operator+=(Matrix const& other)
	{
		for (unsigned int i = 0; i < M; i++)
			for (unsigned int j = 0; j < N; j++)
				m_matrix[i * N + j] += other.m_matrix[i * N + j];
		return *this;
	}

	Matrix& operator-=(Matrix const& other)
	{
		for (unsigned int i = 0; i < M; i++)
			for (unsigned int j = 0; j < N; j++)
				m_matrix[i * N + j] -= other.m_matrix[i * N + j];
		return *this;
	}

	Matrix& operator-()
	{
		for (unsigned int i = 0; i < M; i++)
			for (unsigned int j = 0; j < N; j++)
				m_matrix[i * N + j] = -m_matrix[i * N + j] ;
		return *this;
	}

	template<unsigned int K>
	friend Matrix<T, M, K> operator*(Matrix<T, M, N>const& Lmatrix, Matrix<T, N, K>const& Rmatrix);
	friend Vector<T, M> operator*(Vector<T, M>const& vector, Matrix<T, M, N>const& matrix);
	friend Vector<T, M> operator*(Matrix<T, M, N>const& matrix, Vector<T, M>const& vector);

	inline T const* get_as_pointer() const { return m_matrix.data(); }

	void Transpose()
	{
		Matrix mat = *this;
		for (unsigned int i = 0; i < M; i++)
			for (unsigned int j = 0; j < N; j++)
				m_matrix[i * N + j] = mat[j * N + i];
	}

	static constexpr Matrix make_identity()
	{
		static_assert( (N == M), "requires quadratic matrix");
		return Matrix(std::move(generate_identity()));
	}

private:

	template <class ...Args, typename std::enable_if<(sizeof...(Args) == (M * N)), int>::type = 0>
	static constexpr std::array<T, M * N> generate_identity(Args&& ... args)
	{
		return std::array<T, M * N>{ args... };
	}

	template <class ...Args, typename std::enable_if<(sizeof...(Args) < (M * N)), int>::type = 0>
	static constexpr std::array<T, M * N> generate_identity(Args&&... args)
	{
		if constexpr (sizeof...(Args) / M == sizeof...(Args) % M)
			return generate_identity(args ..., 1.0f);
		else
			return generate_identity(args ..., 0.0f);
	}

	std::array<T, M * N> m_matrix;
};

template<class T>
class Matrix<T, 4, 4>
{
public:
	Matrix() = default;

	template <class ...Args, typename std::enable_if<(sizeof...(Args) == 16), int>::type = 0>
	constexpr Matrix(Args ... args)
		:m_matrix{ args ... }
	{}

	constexpr Matrix(std::array<T, 16> const& mat)
		:m_matrix(mat)
	{}

	constexpr Matrix(std::array<T, 16>&& mat)
		: m_matrix{ std::move(mat) }
	{}
	/*
	Ambiguity in Matrix<T,4, 4> A({something, something, ...}) becouse the compiler can't choose between constexpr Matrix(Args ... args)
	and constexpr  Matrix(std::array<T, 16>&& mat). Instead, you should call Matrix<T, 4, 4> A(something, something, ...) or even
	Matrix<T, 4, 4> A{something, something, ...}.
	*/

	inline T& operator()(unsigned int const row, unsigned int const col) { return m_matrix[row * 4 + col]; }
	inline T const& operator()(unsigned int const row, unsigned int const col) const { return m_matrix[row * 4 + col]; }

	Matrix& operator*=(Matrix const& Rmatrix)
	{
		Matrix aux(*this);
		// row 0
		m_matrix[0] = aux.m_matrix[0] * Rmatrix.m_matrix[0] + aux.m_matrix[1] * Rmatrix.m_matrix[4] + aux.m_matrix[2] * Rmatrix.m_matrix[8] + aux.m_matrix[3] * Rmatrix.m_matrix[12];
		m_matrix[1] = aux.m_matrix[0] * Rmatrix.m_matrix[1] + aux.m_matrix[1] * Rmatrix.m_matrix[5] + aux.m_matrix[2] * Rmatrix.m_matrix[9] + aux.m_matrix[3] * Rmatrix.m_matrix[13];
		m_matrix[2] = aux.m_matrix[0] * Rmatrix.m_matrix[2] + aux.m_matrix[1] * Rmatrix.m_matrix[6] + aux.m_matrix[2] * Rmatrix.m_matrix[10] + aux.m_matrix[3] * Rmatrix.m_matrix[14];
		m_matrix[3] = aux.m_matrix[0] * Rmatrix.m_matrix[3] + aux.m_matrix[1] * Rmatrix.m_matrix[7] + aux.m_matrix[2] * Rmatrix.m_matrix[11] + aux.m_matrix[3] * Rmatrix.m_matrix[15];
		// row 1
		m_matrix[4] = aux.m_matrix[4] * Rmatrix.m_matrix[0] + aux.m_matrix[5] * Rmatrix.m_matrix[4] + aux.m_matrix[6] * Rmatrix.m_matrix[8] + aux.m_matrix[7] * Rmatrix.m_matrix[12];
		m_matrix[5] = aux.m_matrix[4] * Rmatrix.m_matrix[1] + aux.m_matrix[5] * Rmatrix.m_matrix[5] + aux.m_matrix[6] * Rmatrix.m_matrix[9] + aux.m_matrix[7] * Rmatrix.m_matrix[13];
		m_matrix[6] = aux.m_matrix[4] * Rmatrix.m_matrix[2] + aux.m_matrix[5] * Rmatrix.m_matrix[6] + aux.m_matrix[6] * Rmatrix.m_matrix[10] + aux.m_matrix[7] * Rmatrix.m_matrix[14];
		m_matrix[7] = aux.m_matrix[4] * Rmatrix.m_matrix[3] + aux.m_matrix[5] * Rmatrix.m_matrix[7] + aux.m_matrix[6] * Rmatrix.m_matrix[11] + aux.m_matrix[7] * Rmatrix.m_matrix[15];
		// row 2
		m_matrix[8] = aux.m_matrix[8] * Rmatrix.m_matrix[0] + aux.m_matrix[9] * Rmatrix.m_matrix[4] + aux.m_matrix[10] * Rmatrix.m_matrix[8] + aux.m_matrix[11] * Rmatrix.m_matrix[12];
		m_matrix[9] = aux.m_matrix[8] * Rmatrix.m_matrix[1] + aux.m_matrix[9] * Rmatrix.m_matrix[5] + aux.m_matrix[10] * Rmatrix.m_matrix[9] + aux.m_matrix[11] * Rmatrix.m_matrix[13];
		m_matrix[10] = aux.m_matrix[8] * Rmatrix.m_matrix[2] + aux.m_matrix[9] * Rmatrix.m_matrix[6] + aux.m_matrix[10] * Rmatrix.m_matrix[10] + aux.m_matrix[11] * Rmatrix.m_matrix[14];
		m_matrix[11] = aux.m_matrix[8] * Rmatrix.m_matrix[3] + aux.m_matrix[9] * Rmatrix.m_matrix[7] + aux.m_matrix[10] * Rmatrix.m_matrix[11] + aux.m_matrix[11] * Rmatrix.m_matrix[15];
		// row 3
		m_matrix[12] = aux.m_matrix[12] * Rmatrix.m_matrix[0] + aux.m_matrix[13] * Rmatrix.m_matrix[4] + aux.m_matrix[14] * Rmatrix.m_matrix[8] + aux.m_matrix[15] * Rmatrix.m_matrix[12];
		m_matrix[13] = aux.m_matrix[12] * Rmatrix.m_matrix[1] + aux.m_matrix[13] * Rmatrix.m_matrix[5] + aux.m_matrix[14] * Rmatrix.m_matrix[9] + aux.m_matrix[15] * Rmatrix.m_matrix[13];
		m_matrix[14] = aux.m_matrix[12] * Rmatrix.m_matrix[2] + aux.m_matrix[13] * Rmatrix.m_matrix[6] + aux.m_matrix[14] * Rmatrix.m_matrix[10] + aux.m_matrix[15] * Rmatrix.m_matrix[14];
		m_matrix[15] = aux.m_matrix[12] * Rmatrix.m_matrix[3] + aux.m_matrix[13] * Rmatrix.m_matrix[7] + aux.m_matrix[14] * Rmatrix.m_matrix[11] + aux.m_matrix[15] * Rmatrix.m_matrix[15];
		return *this;
	}

	Matrix& operator*=(T const scalar)
	{
		m_matrix[0] *= scalar;
		m_matrix[1] *= scalar;
		m_matrix[2] *= scalar;
		m_matrix[3] *= scalar;

		m_matrix[4] *= scalar;
		m_matrix[5] *= scalar;
		m_matrix[6] *= scalar;
		m_matrix[7] *= scalar;

		m_matrix[8] *= scalar;
		m_matrix[9] *= scalar;
		m_matrix[10] *= scalar;
		m_matrix[11] *= scalar;

		m_matrix[12] *= scalar;
		m_matrix[13] *= scalar;
		m_matrix[14] *= scalar;
		m_matrix[15] *= scalar;
		return *this;
	}

	Matrix& operator/=(T const scalar)
	{
		m_matrix[0] /= scalar;
		m_matrix[1] /= scalar;
		m_matrix[2] /= scalar;
		m_matrix[3] /= scalar;

		m_matrix[4] /= scalar;
		m_matrix[5] /= scalar;
		m_matrix[6] /= scalar;
		m_matrix[7] /= scalar;

		m_matrix[8] /= scalar;
		m_matrix[9] /= scalar;
		m_matrix[10] /= scalar;
		m_matrix[11] /= scalar;

		m_matrix[12] /= scalar;
		m_matrix[13] /= scalar;
		m_matrix[14] /= scalar;
		m_matrix[15] /= scalar;
		return *this;
	}

	Matrix& operator+=(Matrix const& other)
	{
		m_matrix[0] += other.m_matrix[0];
		m_matrix[1] += other.m_matrix[1];
		m_matrix[2] += other.m_matrix[2];
		m_matrix[3] += other.m_matrix[3];

		m_matrix[4] += other.m_matrix[4];
		m_matrix[5] += other.m_matrix[5];
		m_matrix[6] += other.m_matrix[6];
		m_matrix[7] += other.m_matrix[7];

		m_matrix[8] += other.m_matrix[8];
		m_matrix[9] += other.m_matrix[9];
		m_matrix[10] += other.m_matrix[10];
		m_matrix[11] += other.m_matrix[11];

		m_matrix[12] += other.m_matrix[12];
		m_matrix[13] += other.m_matrix[13];
		m_matrix[14] += other.m_matrix[14];
		m_matrix[15] += other.m_matrix[15];
		return *this;
	}

	Matrix& operator-=(Matrix const& other)
	{
		m_matrix[0] -= other.m_matrix[0];
		m_matrix[1] -= other.m_matrix[1];
		m_matrix[2] -= other.m_matrix[2];
		m_matrix[3] -= other.m_matrix[3];

		m_matrix[4] -= other.m_matrix[4];
		m_matrix[5] -= other.m_matrix[5];
		m_matrix[6] -= other.m_matrix[6];
		m_matrix[7] -= other.m_matrix[7];

		m_matrix[8] -= other.m_matrix[8];
		m_matrix[9] -= other.m_matrix[9];
		m_matrix[10] -= other.m_matrix[10];
		m_matrix[11] -= other.m_matrix[11];

		m_matrix[12] -= other.m_matrix[12];
		m_matrix[13] -= other.m_matrix[13];
		m_matrix[14] -= other.m_matrix[14];
		m_matrix[15] -= other.m_matrix[15];
		return *this;
	}

	Matrix& operator-()
	{
		m_matrix[0] = -m_matrix[0];
		m_matrix[1] = -m_matrix[1];
		m_matrix[2] = -m_matrix[2];
		m_matrix[3] = -m_matrix[3];

		m_matrix[4] = -m_matrix[4];
		m_matrix[5] = -m_matrix[5];
		m_matrix[6] = -m_matrix[6];
		m_matrix[7] = -m_matrix[7];

		m_matrix[8] = -m_matrix[8];
		m_matrix[9] = -m_matrix[9];
		m_matrix[10] = -m_matrix[10];
		m_matrix[11] = -m_matrix[11];

		m_matrix[12] = -m_matrix[12];
		m_matrix[13] = -m_matrix[13];
		m_matrix[14] = -m_matrix[14];
		m_matrix[15] = -m_matrix[15];
		return *this;
	}

	friend Vector<T, 4> operator*( Vector<T, 4> const& vector, Matrix<T, 4, 4> const& matrix);
	friend Vector<T, 4> operator*( Matrix<T, 4, 4> const& matrix, Vector<T, 4> const& vector);

	inline T const* get_as_pointer() const { return m_matrix.data(); }

	void Transpose()
	{
		Matrix4x4f mat = *this;
		m_matrix[0] = mat[0];
		m_matrix[1] = mat[4];
		m_matrix[2] = mat[8];
		m_matrix[3] = mat[12];

		m_matrix[4] = mat[1];
		m_matrix[5] = mat[5];
		m_matrix[6] = mat[9];
		m_matrix[7] = mat[13];

		m_matrix[8] = mat[2];
		m_matrix[9] = mat[6];
		m_matrix[10] = mat[10];
		m_matrix[11] = mat[14];

		m_matrix[12] = mat[3];
		m_matrix[13] = mat[7];
		m_matrix[14] = mat[11];
		m_matrix[15] = mat[15];
	}

	static constexpr Matrix make_identity()
	{
		return Matrix( 
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

	static Matrix make_look_at(Vector<T, 3> const& cameraPosition, Vector<T, 3> const& targetPosition, Vector<T, 3> const& up)
	{
		Vector<T, 3> axis_z = cameraPosition - targetPosition;
		axis_z.normalize();
		Vector<T, 3> axis_x = Vector<T, 3>::cross(up, axis_z);
		axis_x.normalize();
		Vector<T, 3> axis_y = Vector<T, 3>::cross(axis_z, axis_x);
		axis_y.normalize();
		return Matrix(
			axis_x[0], axis_y[0], axis_z[0], 0.0f,
			axis_x[1], axis_y[1], axis_z[1], 0.0f,
			axis_x[2], axis_y[2], axis_z[2], 0.0f,
			-Vector<T, 3>::dot(axis_x, cameraPosition), -Vector<T, 3>::dot(axis_y, cameraPosition), -Vector<T, 3>::dot(axis_z, cameraPosition), 1.0f);
	}
	static Matrix make_orthographic_matrix(T const left, T const right, T const top, T const botton, T const _near, T const _far)
	{
		return Matrix(
			2.0f / (right - left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (top - botton), 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / (_far - _near), 0.0f,
			-(right + left) / (right - left), -(top + botton) / (top - botton), -(_far + _near) / (_far - _near), 1.0f);
	}
	static Matrix make_perspective_matrix(T const fov, T const aspect_ratio, T const l_near)
	{
		float const projec_width = std::tan(TO_RADIANTS * fov / 2.0f);
		return Matrix(
			1.0f / (aspect_ratio * projec_width), 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / (projec_width), 0.0f, 0.0f,
			0.0f, 0.0f, -1.0f, -1.0f,
			0.0f, 0.0f, -(2.0f * l_near), 0.0f);
	}

	// translation matrix:
	static Matrix make_translation_matrix(Vector<T, 3> const& tran)
	{
		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			tran[0], tran[1], tran[2], 1.0f);
	}

	inline Vector<T, 3> get_tran_vec() const { return Vector<T, 3>(m_matrix[12], m_matrix[13], m_matrix[14]); }
	inline T get_tran_x() const { return m_matrix[12]; }
	inline T get_tran_y() const { return m_matrix[13]; }
	inline T get_tran_z() const { return m_matrix[14]; }

	inline void set_tran_x(T const tran_x) { m_matrix[12] = tran_x; }
	inline void set_tran_y(T const tran_y) { m_matrix[13] = tran_y; }
	inline void set_tran_z(T const tran_z) { m_matrix[14] = tran_z; }
	inline void set_tran_vec(Vector<T, 3> const& tran) { m_matrix[12] = tran[0]; m_matrix[13] = tran[1]; m_matrix[14] = tran[2]; }

	// Scale matrix.
	static Matrix make_scale_matrix(Vector<T, 3> const& scale)
	{
		return Matrix(
			scale[0], 0.0f, 0.0f, 0.0f,
			0.0f, scale[1], 0.0f, 0.0f,
			0.0f, 0.0f, scale[2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline Vector<T, 3> get_scale_vec() const { return Vector<T, 3>(m_matrix[0], m_matrix[5], m_matrix[10]); }
	inline T get_scale_x() const { return  m_matrix[0]; }
	inline T get_scale_y() const { return m_matrix[5]; }
	inline T get_scale_z() const { return m_matrix[10]; }

	inline void set_scale_x(T const scale_x) { m_matrix[0] = scale_x; }
	inline void set_scale_y(T const scale_y) { m_matrix[5] = scale_y; }
	inline void set_scale_z(T const scale_z) { m_matrix[10] = scale_z; }
	inline void set_scale_vec(Vector<T, 3> const& scale) { m_matrix[0] = scale[0]; m_matrix[5] = scale[1]; m_matrix[10] = scale[2]; }

	// Rotation matrix.
	// X axis
	static Matrix make_rotation_matrix_x(T const angle)
	{
		return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, std::cos(angle), std::sin(angle), 0.0f,
			0.0f, -std::sin(angle), std::cos(angle), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	void set_angle_x(T const angle)
	{
		m_matrix[5] = std::cos(angle);
		m_matrix[6] = std::sin(angle);
		m_matrix[9] = -std::sin(angle);
		m_matrix[10] = std::cos(angle);
	}

	// Y axis
	static Matrix make_rotation_matrix_y(T const angle)
	{
		return Matrix(
			std::cos(angle), 0.0f, -std::sin(angle), 0.0f
			0.0f, 1.0f, 0.0f, 0.0f,
			std::sin(angle), 0.0f, std::cos(angle), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	void set_angle_y(T const angle)
	{
		m_matrix[0] = std::cos(angle);
		m_matrix[2] = -std::sin(angle);
		m_matrix[8] = std::sin(angle);
		m_matrix[10] = std::cos(angle);
	}

	// Z axis
	static Matrix make_rotation_matrix_z(T const angle)
	{
		return Matrix(
			std::cos(angle), std::sin(angle), 0.0f, 0.0f
			-std::sin(angle), std::cos(angle), 0.0f, 0.0f
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	void set_angle_z(T const angle)
	{
		m_matrix[0] = std::cos(angle);
		m_matrix[1] = std::sin(angle);
		m_matrix[4] = -std::sin(angle);
		m_matrix[5] = std::cos(angle);
	}



private:
	std::array<T, 16> m_matrix;
};

template<class T>
class Matrix<T, 3, 3>
{
public:
	Matrix() = default;

	template <class ...Args, typename std::enable_if<(sizeof...(Args) == (9)), int>::type = 0>
	constexpr Matrix(Args ... args)
		:m_matrix{ args ... }
	{}

	constexpr Matrix(std::array<T, 9> const& mat)
		: m_matrix(mat)
	{}

	constexpr Matrix(std::array<T, 9>&& mat)
		: m_matrix{ std::move(mat) }
	{}
	/*
	Ambiguity in Matrix<T, 3, 3> A({something, something, ...}) becouse the compiler can't choose between constexpr Matrix(Args ... args)
	and constexpr  Matrix(std::array<T, 9>&& mat). Instead, you should call Matrix<T, 3, 3> A(something, something, ...) or even
	Matrix<T, 3, 3> A{something, something, ...}.
	*/

	inline T& operator()(unsigned int const row, unsigned int const col) { return m_matrix[row * 3 + col]; }
	inline T const& operator()(unsigned int const row, unsigned int const col) const { return m_matrix[row * 3 + col]; }

	Matrix& operator*=(Matrix const& Rmatrix)
	{
		Matrix aux(*this);
		// row 0
		m_matrix[0] = aux.m_matrix[0] * Rmatrix.m_matrix[0] + aux.m_matrix[1] * Rmatrix.m_matrix[3] + aux.m_matrix[2] * Rmatrix.m_matrix[6];
		m_matrix[1] = aux.m_matrix[0] * Rmatrix.m_matrix[1] + aux.m_matrix[1] * Rmatrix.m_matrix[4] + aux.m_matrix[2] * Rmatrix.m_matrix[7];
		m_matrix[2] = aux.m_matrix[0] * Rmatrix.m_matrix[2] + aux.m_matrix[1] * Rmatrix.m_matrix[5] + aux.m_matrix[2] * Rmatrix.m_matrix[8];
		// row 1
		m_matrix[3] = aux.m_matrix[3] * Rmatrix.m_matrix[0] + aux.m_matrix[4] * Rmatrix.m_matrix[3] + aux.m_matrix[5] * Rmatrix.m_matrix[6];
		m_matrix[4] = aux.m_matrix[3] * Rmatrix.m_matrix[1] + aux.m_matrix[4] * Rmatrix.m_matrix[4] + aux.m_matrix[5] * Rmatrix.m_matrix[7];
		m_matrix[5] = aux.m_matrix[3] * Rmatrix.m_matrix[2] + aux.m_matrix[4] * Rmatrix.m_matrix[5] + aux.m_matrix[5] * Rmatrix.m_matrix[8];
		// row 2
		m_matrix[6] = aux.m_matrix[6] * Rmatrix.m_matrix[0] + aux.m_matrix[7] * Rmatrix.m_matrix[3] + aux.m_matrix[8] * Rmatrix.m_matrix[6];
		m_matrix[7] = aux.m_matrix[6] * Rmatrix.m_matrix[1] + aux.m_matrix[7] * Rmatrix.m_matrix[4] + aux.m_matrix[8] * Rmatrix.m_matrix[7];
		m_matrix[8] = aux.m_matrix[6] * Rmatrix.m_matrix[2] + aux.m_matrix[7] * Rmatrix.m_matrix[5] + aux.m_matrix[8] * Rmatrix.m_matrix[8];
		return *this;
	}

	Matrix& operator*=(T const scalar)
	{
		m_matrix[0] *= scalar;
		m_matrix[1] *= scalar;
		m_matrix[2] *= scalar;

		m_matrix[3] *= scalar;
		m_matrix[4] *= scalar;
		m_matrix[5] *= scalar;

		m_matrix[6] *= scalar;
		m_matrix[7] *= scalar;
		m_matrix[8] *= scalar;
		return *this;
	}

	Matrix& operator/=(T const scalar)
	{
		m_matrix[0] /= scalar;
		m_matrix[1] /= scalar;
		m_matrix[2] /= scalar;

		m_matrix[3] /= scalar;
		m_matrix[4] /= scalar;
		m_matrix[5] /= scalar;

		m_matrix[6] /= scalar;
		m_matrix[7] /= scalar;
		m_matrix[8] /= scalar;
		return *this;
	}

	Matrix& operator+=(Matrix const& other)
	{
		m_matrix[0] += other.m_matrix[0];
		m_matrix[1] += other.m_matrix[1];
		m_matrix[2] += other.m_matrix[2];
		
		m_matrix[3] += other.m_matrix[3];
		m_matrix[4] += other.m_matrix[4];
		m_matrix[5] += other.m_matrix[5];

		m_matrix[6] += other.m_matrix[6];
		m_matrix[7] += other.m_matrix[7];
		m_matrix[8] += other.m_matrix[8];
		return *this;
	}

	Matrix& operator-=(Matrix const& other)
	{
		m_matrix[0] -= other.m_matrix[0];
		m_matrix[1] -= other.m_matrix[1];
		m_matrix[2] -= other.m_matrix[2];
		
		m_matrix[3] -= other.m_matrix[3];
		m_matrix[4] -= other.m_matrix[4];
		m_matrix[5] -= other.m_matrix[5];
		
		m_matrix[6] -= other.m_matrix[6];
		m_matrix[7] -= other.m_matrix[7];
		m_matrix[8] -= other.m_matrix[8];
		return *this;
	}

	Matrix& operator-()
	{
		m_matrix[0] = -m_matrix[0];
		m_matrix[1] = -m_matrix[1];
		m_matrix[2] = -m_matrix[2];
		
		m_matrix[3] = -m_matrix[3];
		m_matrix[4] = -m_matrix[4];
		m_matrix[5] = -m_matrix[5];
		
		m_matrix[6] = -m_matrix[6];
		m_matrix[7] = -m_matrix[7];
		m_matrix[8] = -m_matrix[8];
		return *this;
	}

	friend Vector<T, 3> operator*(Vector<T, 3> const& vector, Matrix<T, 3, 3> const& matrix);
	friend Vector<T, 3> operator*(Matrix<T, 3, 3> const& matrix, Vector<T, 3> const& vector);

	inline T const* get_as_pointer() const { return m_matrix.data(); }

	void Transpose()
	{
		Matrix4x4f mat = *this;
		m_matrix[0] = mat[0];
		m_matrix[1] = mat[3];
		m_matrix[2] = mat[6];
		
		m_matrix[3] = mat[1];
		m_matrix[4] = mat[4];
		m_matrix[5] = mat[7];
		
		m_matrix[6] = mat[2];
		m_matrix[7] = mat[5];
		m_matrix[8] = mat[8];
	}

	static constexpr Matrix make_identity()
	{
		return Matrix(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	static Matrix make_orthographic_matrix(T const left, T const right, T const top, T const bottom)
	{
		return Matrix(
			2.0f / (right + left), 0.0f, 0.0f,
			0.0f, 2.0f / (top + bottom), 0.0f,
			-(right - left) / (right + left), -(top - bottom) / (top + bottom), 1.0f);
	}

	// translation matrix:
	static Matrix make_translation_matrix(Vector<T, 2> const& tran)
	{
		return Matrix(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			tran[0], tran[1], 1.0f);
	}
	
	inline Vector<T, 2> get_tran_vec() const { return Vector<T, 2>(m_matrix[6], m_matrix[7]); }
	inline T get_tran_x() const { return m_matrix[6]; }
	inline T get_tran_y() const { return m_matrix[7]; }

	inline void set_tran_x(T const tran_x) { m_matrix[6] = tran_x; }
	inline void set_tran_y(T const tran_y) { m_matrix[7] = tran_y; }
	inline void set_tran_vec(Vector<T, 2> const& tran) { m_matrix[6] = tran[0]; m_matrix[7] = tran[1]; }
	
	// Scale matrix.
	static Matrix make_scale_matrix(Vector<T, 2> const& scale)
	{
		return Matrix(
			scale[0], 0.0f, 0.0f,
			0.0f, scale[1], 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	inline Vector<T, 2> get_scale_vec() const { return Vector<T, 2>(m_matrix[0], m_matrix[4]); }
	inline T get_scale_x() const { return m_matrix[0]; }
	inline T get_scale_y() const { return m_matrix[4]; }

	inline void set_scale_x(T const scale_x) { m_matrix[0] = scale_x; }
	inline void set_scale_y(T const scale_y) { m_matrix[4] = scale_y; }
	inline void set_scale_vec(Vector<T, 2> const& scale) { (m_matrix[0] = scale[0]; m_matrix[4] = scale[1];}
	
	// Rotation matrix.
	// X axis
	static Matrix make_rotation_matrix_x(T const angle)
	{
		return matrix(
			1.0f, 0.0f, 0.0f,
			0.0f, std::cos(angle), std::sin(angle),
			0.0f, -std::sin(angle), std::cos(angle));

		)
	}
	void set_angle_x(T const angle)
	{
		m_matrix[4] = std::cos(angle);
		m_matrix[5] = std::sin(angle);
		m_matrix[7] = -std::sin(angle);
		m_matrix[8] = std::cos(angle);
	}

	// Y axis
	static Matrix make_rotation_matrix_y(T const angle)
	{
		return Matrix(
			std::cos(angle), 0.0f, -std::sin(angle),
			0.0f, 1.0f, 0.0f,
			std::sin(angle), 0.0f, std::cos(angle));
	}
	void set_angle_y(T const angle)
	{
		m_matrix[0] = std::cos(angle);
		m_matrix[2] = -std::sin(angle);
		m_matrix[6] = std::sin(angle);
		m_matrix[8] = std::cos(angle);
	}

	// Z axis
	static Matrix make_rotation_matrix_z(T const angle)
	{		
		return Matrix(
			std::cos(angle), std::sin(angle), 0.0f,
			-std::sin(angle), std::cos(angle), 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	void set_angle_z(T const angle)
	{
		m_matrix[0] = std::cos(angle);
		m_matrix[1] = std::sin(angle);
		m_matrix[3] = -std::sin(angle);
		m_matrix[4] = std::cos(angle);
	}

private:
	std::array<T, 9> m_matrix;
};


// Matrix x Matrix
template<class T, unsigned int N>
Matrix<T, N, N> operator*(Matrix<T, N, N> Lmatrix, Matrix<T, N, N> const& Rmatrix)
{
	return Lmatrix *= Rmatrix;
}

template<class T, unsigned int M, unsigned int N, unsigned int K,
	typename std::enable_if< (M != N || N != K), int>::type = 0>
	Matrix<T, M, K> operator*(Matrix<T, M, N> const& Lmatrix, Matrix<T, N, K> const& Rmatrix)
{
	Matrix<T, M, K> result;
	for (unsigned int i = 0; i < M; i++)
		for (unsigned int j = 0; j < K; j++)
			for (unsigned int x = 0; x < N; x++)
				result.m_matrix[i * K + j] += Lmatrix.m_matrix[i * N + x] * Rmatrix.m_matrix[x * (i * K) + j];
	return result;
}

// Scalar mutiplication and division.

template<class T, unsigned int M, unsigned int N>
Matrix<T, M, N> operator*(Matrix<T, M, N> matrix, T scalar)
{
	return matrix *= scalar;
}

template<class T, unsigned int M, unsigned int N>
Matrix<T, M, N> operator*(T scalar, Matrix<T, M, N> matrix)
{
	return matrix *= scalar;
}

template<class T, unsigned int M, unsigned int N>
Matrix<T, M, N> operator/(Matrix<T, M, N> matrix, T scalar)
{
	return matrix /= scalar;
}

template<class T, unsigned int M, unsigned int N>
Matrix<T, M, N> operator/(T scalar, Matrix<T, M, N> matrix)
{
	return matrix /= scalar;
}

// Matrix arithmetics.

template<class T, unsigned int M, unsigned int N>
Matrix<T, M, N> operator+(Matrix<T, M, N> matrix1, Matrix<T, M, N> const& matrix2)
{
	return matrix1 += matrix2;
}

template<class T, unsigned int M, unsigned int N>
Matrix<T, M, N> operator-(Matrix<T, M, N> matrix1, Matrix<T, M, N> const& matrix2)
{
	return matrix1 -= matrix2;
}

// Vector x Matrix mutiplication.

template<class T, unsigned int M, unsigned int N>
Vector<T, M> operator*(Vector<T, M> const& vector, Matrix<T, M, N> const& matrix)
{
	Vector<T, N> result;
	for (unsigned int i = 0; i < N; i++)
		for (unsigned int j = 0; j < M; j++)
			result[i] += vector[j] * matrix.m_matrix[i * N + j];
		return result;
}

template<class T, unsigned int M, unsigned int N>
Vector<T, M> operator*(Matrix<T, M, N> const& matrix, Vector<T, N>const& vector)
{
	Vector<T, N> result;
	for (unsigned int i = 0; i < M; i++)
		for (unsigned int j = 0; j < N; j++)
			result[i] += vector[j] * matrix.m_matrix[j * N + i];
	return result;
}

template<class T>
Vector<T, 4> operator*(Vector<T, 4> const& vector, Matrix<T, 4, 4> const& matrix)
{
	Vector<T, 4> result;
	result[0] = vector[0] * matrix.m_matrix[0] + vector[1] * matrix.m_matrix[4] + vector[2] * matrix.m_matrix[8] + vector[3] * matrix.m_matrix[12];
	result[1] = vector[0] * matrix.m_matrix[1] + vector[1] * matrix.m_matrix[5] + vector[2] * matrix.m_matrix[9] + vector[3] * matrix.m_matrix[13];
	result[2] = vector[0] * matrix.m_matrix[2] + vector[1] * matrix.m_matrix[6] + vector[2] * matrix.m_matrix[10] + vector[3] * matrix.m_matrix[14];
	result[3] = vector[0] * matrix.m_matrix[3] + vector[1] * matrix.m_matrix[7] + vector[2] * matrix.m_matrix[11] + vector[3] * matrix.m_matrix[15];
	return result;
}

template<class T>
Vector<T, 4> operator*(Matrix<T, 4, 4> const& matrix, Vector<T, 4> const& vector)
{
	Vector<T, 3> result;
	result[0] = vector[0] * matrix.m_matrix[0] + vector[1] * matrix.m_matrix[1] + vector[2] * matrix.m_matrix[2] + vector[3] * matrix.m_matrix[3];
	result[1] = vector[0] * matrix.m_matrix[4] + vector[1] * matrix.m_matrix[5] + vector[2] * matrix.m_matrix[6] + vector[3] * matrix.m_matrix[7];
	result[2] = vector[0] * matrix.m_matrix[8] + vector[1] * matrix.m_matrix[9] + vector[2] * matrix.m_matrix[10] + vector[3] * matrix.m_matrix[11];
	result[3] = vector[0] * matrix.m_matrix[12] + vector[1] * matrix.m_matrix[13] + vector[2] * matrix.m_matrix[14] + vector[3] * matrix.m_matrix[15];
	return result;
}

template<class T>
Vector<T, 3> operator*(Vector<T, 3> const& vector, Matrix<T, 3, 3> const& matrix)
{
	Vector<T, 3> result;
	result[0] = vector[0] * matrix.m_matrix[0] + vector[1] * matrix.m_matrix[3] + vector[2] * matrix.m_matrix[6];
	result[1] = vector[0] * matrix.m_matrix[1] + vector[1] * matrix.m_matrix[4] + vector[2] * matrix.m_matrix[7];
	result[2] = vector[0] * matrix.m_matrix[2] + vector[1] * matrix.m_matrix[5] + vector[2] * matrix.m_matrix[8];
	return result;
}

template<class T>
Vector<T, 3> operator*(Matrix<T, 3, 3> const& matrix, Vector<T, 3> const& vector)
{
	Vector<T, 3> result;
	result[0] = vector[0] * matrix.m_matrix[0] + vector[1] * matrix.m_matrix[1] + vector[2] * matrix.m_matrix[2];
	result[1] = vector[0] * matrix.m_matrix[3] + vector[1] * matrix.m_matrix[4] + vector[2] * matrix.m_matrix[5];
	result[2] = vector[0] * matrix.m_matrix[6] + vector[1] * matrix.m_matrix[7] + vector[2] * matrix.m_matrix[8];
	return result;
}

// Cout
template<class T, unsigned int M, unsigned int N>
std::ostream& operator<<(std::ostream& os, Matrix<T, M, N>& matrix)
{
	unsigned int aux = N - 1;
	for (unsigned int i = 0; i < M; i++)
		for (unsigned int j = 0; j < N; j++)
			if (j == 0)
				os << "{\t" << matrix.GetElement(i, j);
			else if (j == aux)
				os << ",\t" << matrix.GetElement(i, j) << "\t}\n";
			else
				os << ",\t" << matrix.GetElement(i, j);
	return os;
}

typedef Matrix<int, 4, 4> Matrix4x4i;
typedef Matrix<float, 4, 4> Matrix4x4f;
typedef Matrix<double, 4, 4> Matrix4x4d;
typedef Matrix<int, 3, 3> Matrix3x3i;
typedef Matrix<float, 3, 3> Matrix3x3f;
typedef Matrix<double, 3, 3> Matrix3x3d;
typedef Matrix<int, 2, 2> Matrix2x2i;
typedef Matrix<float, 2, 2> Matrix2x2f;
typedef Matrix<double, 2, 2> Matrix2x2d;

constexpr Matrix4x4f IDENTITY_4x4F = Matrix4x4f::make_identity();
constexpr Matrix3x3f IDENTITY_3x3F = Matrix3x3f::make_identity();
constexpr Matrix2x2f IDENTITY_2x2F = Matrix2x2f::make_identity();
