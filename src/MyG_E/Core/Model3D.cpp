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
	: mPosition({ 0.0f, 0.0f, 0.0f })
	, mTransMatrix(0.0f, 0.0f, 0.0f)
	, mScaleMatrix(1.0f, 1.0f, 1.0f)
	, mScale({ 1.0f, 1.0f, 1.0f })
	, mRotationMatrix(Matrix<float, 4, 4>::make_identity())
	, mMesh(mesh)
	, m_is_visible(true)
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
	mObjectName(name),
	m_is_visible(true)
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
	Quaternion quatx = Quaternion::MakeRotate(mRotate[0], { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::MakeRotate(mRotate[1], { 0.0f, 1.0f, 0.0f });
	Quaternion quatz = Quaternion::MakeRotate(mRotate[2], { 0.0f, 0.0f, 1.0f });
	Quaternion quat = quatx * quaty * quatz;

	mRotationMatrix = Quaternion::CreateRotationMatrix(quat);
}

void Model3D::SetMaterial(Vector<float, 3>& ambient, Vector<float, 3>& diffuse, Vector<float, 3>& specular, float shininess)
{
	mMaterial.ambient = ambient;
	mMaterial.diffuse = diffuse;
	mMaterial.specular = specular;
	mMaterial.shininess = shininess;
}

void Model3D::ImGuiRenderer()
{
	ImGui::SliderFloat3("Translate", &mPosition[0], -50.0f, 50.0f);
	ImGui::SliderFloat3("Scale", &mScale[0], 0.0f, 5.0f);
	ImGui::SliderFloat3("Rotate", &mRotate[0], -6.28f, 6.28f);
	ImGui::ColorEdit3("Ambient", &mMaterial.ambient[0]);
	ImGui::ColorEdit3("Diffuse", &mMaterial.diffuse[0]);
	ImGui::ColorEdit3("Specular", &mMaterial.specular[0]);
	ImGui::SliderFloat("shininess", &mMaterial.shininess, 0.0f, 1.0f);
	ImGui::Checkbox("Visibility", &m_is_visible);

	SetTranslation(mPosition);
	SetScale(mScale);
	SetRotation(mRotate);
}

