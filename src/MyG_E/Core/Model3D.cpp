// Heap file.
#include "Model3D.h"

// Intern files.
#include "Foundation\Math\Quaternion.h"

// std fiels
#include <string>

// Third Parties.
#include "imgui.h"

unsigned int Model3D::mNumberofObjects = 0;

Model3D::Model3D(std::shared_ptr<Mesh> mesh)
	:mPosition({ 0.0f, 0.0f, 0.0f }),
	mTransMatrix(0.0f, 0.0f, 0.0f),
	mScaleMatrix(1.0f, 1.0f, 1.0f),
	mScale({ 1.0f, 1.0f, 1.0f }),
	mRotationMatrix(Matrix<float, 4, 4>::make_identity()),
	mMesh(mesh)
{
	mObjectName = "Object " + std::to_string(mNumberofObjects);
	mNumberofObjects++;
}

Model3D::Model3D(std::shared_ptr<Mesh> mesh, const std::string& name)
	:mPosition({ 0.0f, 0.0f, 0.0f }),
	mTransMatrix(0.0f, 0.0f, 0.0f),
	mScaleMatrix(1.0f, 1.0f, 1.0f),
	mScale({1.0f, 1.0f, 1.0f}),
	mRotationMatrix(),
	mMesh(mesh),
	mObjectName(name)
{
	mNumberofObjects++;
}

Model3D::~Model3D()
{
}

void Model3D::SetTranslation(const Vector<float, 3>& trans)
{
	mPosition = trans;
	mTransMatrix.SetTranX(mPosition[0]);
	mTransMatrix.SetTranY(mPosition[1]);
	mTransMatrix.SetTranZ(mPosition[2]);
}

void Model3D::SetScale(const Vector<float, 3>& scale)
{
	mScale = scale;
	mScaleMatrix.SetScaleX(mScale[0]);
	mScaleMatrix.SetScaleY(mScale[1]);
	mScaleMatrix.SetScaleZ(mScale[2]);
}

void Model3D::SetRotation(const Vector<float, 3>& rotation)
{
	mRotate = rotation;
	Quaternion quatx = Quaternion::make_rotate(mRotate[0], { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::make_rotate(mRotate[1], { 0.0f, 1.0f, 0.0f });
	Quaternion quatz = Quaternion::make_rotate(mRotate[2], { 0.0f, 0.0f, 1.0f });
	Quaternion quat = quatx * quaty * quatz;

	mRotationMatrix = Quaternion::create_rotation_matrix(quat);
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

