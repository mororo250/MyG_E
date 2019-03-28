#include "Model3D.h"

Model3D::Model3D(const Vector<float, 3> Dimensions, const std::vector<Vector<float, 4>> Color)
	:mPosition({0.0f, 0.0f, 0.0f}),
	mTransMatrix(0.0f, 0.0f, 0.0f),
	mScaleMatrix(1.0f, 1.0f, 1.0f),
	mRotationMatrix()
{
	mVertexData.reserve(8);
	VertexData3C aux;
	//down-left-back
	aux.Position = {  -Dimensions[0], -Dimensions[1], -Dimensions[2] };
	aux.Color = Color[0];
	mVertexData.push_back(aux);
	//down-right-back
	aux.Position = {Dimensions[0], -Dimensions[1], -Dimensions[2] };
	aux.Color = Color[1];
	mVertexData.push_back(aux);
	//up-right-back
	aux.Position = {Dimensions[0], Dimensions[1], -Dimensions[2] };
	aux.Color = Color[2];
	mVertexData.push_back(aux);
	//up-left-back
	aux.Position = { -Dimensions[0], Dimensions[1], -Dimensions[2] };
	aux.Color = Color[3];
	mVertexData.push_back(aux);
	//down-left-front
	aux.Position = {-Dimensions[0], -Dimensions[1], Dimensions[2] };
	aux.Color = Color[4];
	mVertexData.push_back(aux);
	//down-right-front
	aux.Position = {Dimensions[0], -Dimensions[1], Dimensions[2] };
	aux.Color = Color[5];
	mVertexData.push_back(aux);
	//up-right-front
	aux.Position = { Dimensions[0], Dimensions[1], Dimensions[2] };
	aux.Color = Color[6];
	mVertexData.push_back(aux);
	//up-left-front
	aux.Position = { -Dimensions[0], Dimensions[1], Dimensions[2] };
	aux.Color = Color[7];
	mVertexData.push_back(aux);
}

Model3D::Model3D(const Vector<float, 3> Dimensions, const Vector<float, 4> Color)
	:mPosition({ 0.0f, 0.0f, 0.0f}),
	mTransMatrix(0.0f, 0.0f, 0.0f),
	mScaleMatrix(1.0f, 1.0f, 1.0f),
	mRotationMatrix()
{
	mVertexData.reserve(8);
	VertexData3C aux;

	aux.Position = { -Dimensions[0], -Dimensions[1], -Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);

	aux.Position = { Dimensions[0], -Dimensions[1], -Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);

	aux.Position = { Dimensions[0], Dimensions[1], Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);

	aux.Position = { -Dimensions[0], Dimensions[1], -Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);

	aux.Position = { -Dimensions[0], -Dimensions[1], Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);

	aux.Position = { Dimensions[0], -Dimensions[1], Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);

	aux.Position = { Dimensions[0], Dimensions[1], Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);

	aux.Position = { -Dimensions[0], Dimensions[1], Dimensions[2] };
	aux.Color = Color;
	mVertexData.push_back(aux);
}

Model3D::Model3D(Vector<float, 3> Position)
	:mPosition(Position),
	mTransMatrix(Position[0], Position[1], Position[2]),
	mScaleMatrix(1.0f, 1.0f, 1.0f),
	mRotationMatrix()
{
}

void Model3D::SetTranslation(Vector<float, 3> trans)
{
	mPosition = trans;
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

