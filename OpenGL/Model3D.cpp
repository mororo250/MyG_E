#include "Model3D.h"

Model3D::Model3D(Vector<float, 3> Position, Vector<float, 3> Dimensions, Vector<float, 4> Color)
	:mPosition(Position),
	mTransMatrix(0.0f, 0.0f, 0.0f),
	mScaleMatrix(1.0f, 1.0f, 1.0f)
{
	mColorData.reserve(8);
	VertexData3C aux;

	aux.Position = { Position[0] - Dimensions[0], Position[1] - Dimensions[1], Position[2] - Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
	Color = { 0.0, 0.0, 1.0, 1.0 };
	aux.Position = { Position[0] + Dimensions[0], Position[1] - Dimensions[1], Position[2] - Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
	Color = { 0.0, 1.0, 0.0, 1.0 };
	aux.Position = { Position[0] + Dimensions[0], Position[1] + Dimensions[1], Position[2] - Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
	Color = { 1.0, 0.0, 0.0, 1.0 };
	aux.Position = { Position[0] - Dimensions[0], Position[1] + Dimensions[1], Position[2] - Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
	Color = { 1.0, 1.0, 1.0, 1.0 };
	aux.Position = { Position[0] - Dimensions[0], Position[1] - Dimensions[1], Position[2] + Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
	Color = { 0.0, 0.0, 1.0, 1.0 };
	aux.Position = { Position[0] + Dimensions[0], Position[1] - Dimensions[1], Position[2] + Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
	Color = { 0.0, 1.0, 0.0, 1.0 };
	aux.Position = { Position[0] + Dimensions[0], Position[1] + Dimensions[1], Position[2] + Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
	Color = { 0.0, 0.0, 1.0, 1.0 };
	aux.Position = { Position[0] - Dimensions[0], Position[1] + Dimensions[1], Position[2] + Dimensions[2] };
	aux.Color = Color;
	mColorData.push_back(aux);
}

void Model3D::SetTranslation(Vector<float, 3> trans)
{
	mTransMatrix.SetTranX(trans[0]);
	mTransMatrix.SetTranY(trans[1]);
	mTransMatrix.SetTranZ(trans[2]);
}

void Model3D::SetScale(Vector<float, 3> scale)
{
	mScaleMatrix.SetScaleX(scale[0]);
	mScaleMatrix.SetScaleY(scale[1]);
	mScaleMatrix.SetScaleZ(scale[2]);
}

void Model3D::SetRotation(Vector<float, 3> Rotation)
{
	mRotationMatrix.RotateXYZ(Rotation[0], Rotation[1], Rotation[2]);
}

