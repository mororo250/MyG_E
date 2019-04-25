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
		std::memcpy(mVector, vector, NumElem * sizeof(T));
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

	//operators
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

	inline bool operator==(const Vector& other){ return std::equal(mVector, mVector + NumElem, other.mVector); }

	template<class U, unsigned int VNumElem, unsigned int MNumCol>
	friend Vector<U, MNumCol> operator*(const Vector<U, VNumElem> vector, const Matrix<U, VNumElem, MNumCol> matrix);

	//auxiliares fuctions
	auto GetAsPointer() const { return mVector; }
	unsigned int GetNumElem() const { return NumElem; }

	float Length()
	{
		float length = 0;
		for (unsigned int i = 0; i < NumElem; i++)
			length += mVector[i] * mVector[i];
		return sqrt(length);
	}

	//return normalized vector but dont normalize it
	Vector<T, NumElem> GetNormalized()
	{
		Vector<T, NumElem> aux = *this;
		aux.Normalize();
		return aux;
	}

	Vector<T, NumElem> Normalize()
	{
		float L = Length();
		for (unsigned int i = 0; i < NumElem; i++)
			mVector[i] = mVector[i] / L;
		return *this;
	}

private:
	T mVector[NumElem];
};

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
Vector<T, NumElem> operator+(Vector<T, NumElem> vector1, Vector<T, NumElem> vector2)
{
	return vector1 += vector2;
}

template<class T, unsigned int NumElem>
Vector<T, NumElem> operator-(Vector<T, NumElem> vector1, Vector<T, NumElem> vector2)
{
	return vector1 -= vector2;
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

//Scalar product and vector product
Vector<float, 3> Cross(Vector<float, 3> vector1, const Vector<float, 3> vector2);

template <class T, unsigned int NumElem>
T Dot(const Vector<T, NumElem> vector1, const Vector<T, NumElem> vector2)
{
	T result = 0;
	for (unsigned int i = 0; i < NumElem; i++)
		result+= vector1[i] * vector2[i];
	return result;
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
		std::memcpy(mMatrix, mat, NumRow * NumCol * sizeof(T));
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
	template<class U, unsigned int VNumElem, unsigned int MNumCol>
	friend Vector<U, MNumCol> operator*(const Vector<U, VNumElem> vector, const Matrix<U, VNumElem, MNumCol> matrix);

	auto GetAsPointer() const { return mMatrix; }
	T& GetElement(const unsigned int index1, const unsigned int index2) { return mMatrix[index1][index2]; }
	unsigned int GetNumRow() const { return NumRow; }
	unsigned int GetNumCol() const { return NumCol; }

	Matrix<T, NumRow, NumCol> Transpose()
	{
		for (unsigned int i = 0; i < NumRow; i++)
			for (unsigned int j = 0; j < NumCol; j ++ )
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

//cout
template<class T, unsigned int NumRow, unsigned int NumCol>
std::ostream& operator<<(std::ostream& os, const Matrix<T, NumRow, NumCol>& matrix)
{
	unsigned int aux = NumCol - 1;
	for (unsigned int i = 0; i < NumRow; i++)
		for (unsigned int j = 0; j < NumCol; j++)
			if (j == 0)
				os << "{\t" << matrix.GetElement(i,j);
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
	EulerAngles();

	void RotateX(float angle) { mRotX.SetAngle(angle); }
	void RotateY(float angle) { mRotY.SetAngle(angle); }
	void RotateZ(float angle) { mRotZ.SetAngle(angle); }
	void RotateXYZ(float angleX, float angleY, float angleZ);

	Matrix<float, 4, 4> GetRotation() { return mRotZ * mRotY * mRotX; }

private:
	RotationMatrix4 mRotX;
	RotationMatrix4 mRotY;
	RotationMatrix4 mRotZ;
};

//fuctions which create a matrix
inline float ToRadiants(float angle);

Matrix<float, 3, 3> CreateOrthoMatrix(const float left, const float right, const float top, const float bottom);
Matrix<float, 4, 4> LookAt(const Vector<float , 3> cameraPosition, const Vector<float , 3> targetPosition, const Vector<float, 3>up);
Matrix<float, 4, 4> CreateOrthographicMatrix(const float left, const float right, const float top, const float botton, const float _near, const float _far);
Matrix<float, 4, 4> CreatePerspectiveMatrix(const float fov, const float aspectRatio, const float Near, const float far);