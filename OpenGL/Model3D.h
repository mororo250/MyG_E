#pragma once

#include "Sprite.h"
#include "Math.h"
class Model3D
{
public:
	Model3D(Vector<float, 3> Position, Vector<float, 3> Dimensions, Vector<float, 4> Color);
	
	VertexData3C* GetData() { return mColorData.data(); }
	Vector<float, 3> GetPosition() { return mPosition; };

	TranslationMatrix4 GetTranslation() { return mTransMatrix; }
	void SetTranslation(Vector<float,3> trans);

	ScaleMatrix4 GetScale() { return mScaleMatrix; }
	void SetScale(Vector<float,3> Scale);

	Matrix<float, 4, 4> GetRotation() { return mRotationMatrix.GetRotation(); }
	void SetRotation(Vector<float, 3> Rotation);

private:
	Vector<float , 3> mPosition;
	std::vector<VertexData3C> mColorData; 
	
	TranslationMatrix4 mTransMatrix;
	ScaleMatrix4 mScaleMatrix;
	EulerAngles mRotationMatrix;
};