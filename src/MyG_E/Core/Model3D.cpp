#include "Model3D.h"
#include <string>
#include "imgui.h"

unsigned int Model3D::mNumberofObjects = 0;

Model3D::Model3D(std::shared_ptr<Mesh> mesh)
	:mPosition({ 0.0f, 0.0f, 0.0f }),
	mTransMatrix(0.0f, 0.0f, 0.0f),
	mScaleMatrix(1.0f, 1.0f, 1.0f),
	mRotationMatrix(),
	mMesh(mesh)
{
	mObjectName = "Object " + std::to_string(mNumberofObjects);
	mNumberofObjects++;
}

Model3D::Model3D(std::shared_ptr<Mesh> mesh, const std::string& name)
	:mPosition({ 0.0f, 0.0f, 0.0f }),
	mTransMatrix(0.0f, 0.0f, 0.0f),
	mScaleMatrix(1.0f, 1.0f, 1.0f),
	mRotationMatrix(),
	mMesh(mesh),
	mObjectName(name)
{
	mNumberofObjects++;
}

Model3D::~Model3D()
{
}

void Model3D::SetTranslation(Vector<float, 3> trans)
{
	mPosition = trans;
	mTransMatrix.SetTranX(mPosition[0]);
	mTransMatrix.SetTranY(mPosition[1]);
	mTransMatrix.SetTranZ(mPosition[2]);
}

void Model3D::SetScale(Vector<float, 3> scale)
{
	mScale = scale;
	mScaleMatrix.SetScaleX(mScale[0]);
	mScaleMatrix.SetScaleY(mScale[1]);
	mScaleMatrix.SetScaleZ(mScale[2]);
}

void Model3D::SetRotation(Vector<float, 3> rotation)
{
	mRotate = rotation;
	mRotationMatrix.RotateXYZ(mRotate[0], mRotate[1], mRotate[2]);
}

void Model3D::ImGuiRenderer()
{
	ImGui::SliderFloat3("Translate", &mPosition[0], -50.0f, 50.0f);
	ImGui::SliderFloat3("Scale", &mScale[0], 0.0f, 5.0f);
	ImGui::SliderFloat3("Rotate", &mRotate[0], -6.28f, 6.28f);
	
	SetTranslation(mPosition);
	SetScale(mScale);
	SetRotation(mRotate);
}

