#pragma once

#include "Sprite.h"
#include "Math.h"
class Model3D
{
public:
	Model3D(const Vector<float, 3> Dimensions, const std::vector<Vector<float, 4>> Color);
	Model3D(const Vector<float, 3> Dimensions, const Vector<float, 4> Color);//one color in all the cube
	Model3D(Vector<float, 3> Position); //intances
 	
	inline VertexData3C* GetData() { return mVertexData.data(); }
	inline Vector<float, 3> GetPosition() const { return mPosition; };

	inline TranslationMatrix4 GetTranslation() const { return mTransMatrix; }
	void SetTranslation(Vector<float,3> trans);

	inline ScaleMatrix4 GetScale() const { return mScaleMatrix; }
	void SetScale(Vector<float,3> Scale);

	inline Matrix<float, 4, 4> GetRotation() { return mRotationMatrix.GetRotation(); }
	void SetRotation(Vector<float, 3> Rotation);


private:
	Vector<float , 3> mPosition;
	std::vector<VertexData3C> mVertexData;

	TranslationMatrix4 mTransMatrix;
	ScaleMatrix4 mScaleMatrix;
	EulerAngles mRotationMatrix;
};