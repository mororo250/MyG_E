// Heap file.
#include "Model3D.h"

// Intern files.
#include "Foundation\Math\Quaternion.h"

// std fiels
#include <string>

// Third Parties.
#include "imgui.h"

unsigned int Model3D::m_number_of_objects = 0;

Model3D::Model3D(std::shared_ptr<Mesh> mesh)
	: m_position({ 0.0f, 0.0f, 0.0f })
	, m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_scale({ 1.0f, 1.0f, 1.0f })
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
	, m_mesh(mesh)
	, m_is_visible(true)
{
	m_object_name = "Object " + std::to_string(m_number_of_objects);
	m_number_of_objects++;
}

Model3D::Model3D(std::shared_ptr<Mesh> mesh, const std::string& name)
	:m_position({ 0.0f, 0.0f, 0.0f }),
	m_trans_matrix(0.0f, 0.0f, 0.0f),
	m_scale_matrix(1.0f, 1.0f, 1.0f),
	m_scale({1.0f, 1.0f, 1.0f}),
	m_rotation_matrix(),
	m_mesh(mesh),
	m_object_name(name),
	m_is_visible(true)
{
	m_number_of_objects++;
}

Model3D::~Model3D()
{
}

void Model3D::SetTranslation(const Vector<float, 3>& trans)
{
	m_position = trans;
	m_trans_matrix.SetTranX(m_position[0]);
	m_trans_matrix.SetTranY(m_position[1]);
	m_trans_matrix.SetTranZ(m_position[2]);
}

void Model3D::SetScale(const Vector<float, 3>& scale)
{
	m_scale = scale;
	m_scale_matrix.SetScaleX(m_scale[0]);
	m_scale_matrix.SetScaleY(m_scale[1]);
	m_scale_matrix.SetScaleZ(m_scale[2]);
}

void Model3D::SetRotation(const Vector<float, 3>& rotation)
{
	m_rotate = rotation;
	Quaternion quatx = Quaternion::MakeRotate(m_rotate[0], { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::MakeRotate(m_rotate[1], { 0.0f, 1.0f, 0.0f });
	Quaternion quatz = Quaternion::MakeRotate(m_rotate[2], { 0.0f, 0.0f, 1.0f });
	Quaternion quat = quatx * quaty * quatz;

	m_rotation_matrix = Quaternion::CreateRotationMatrix(quat);
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
	ImGui::SliderFloat3("Translate", &m_position[0], -50.0f, 50.0f);
	ImGui::SliderFloat3("Scale", &m_scale[0], 0.0f, 5.0f);
	ImGui::SliderFloat3("Rotate", &m_rotate[0], -6.28f, 6.28f);
	ImGui::ColorEdit3("Ambient", &mMaterial.ambient[0]);
	ImGui::ColorEdit3("Diffuse", &mMaterial.diffuse[0]);
	ImGui::ColorEdit3("Specular", &mMaterial.specular[0]);
	ImGui::SliderFloat("shininess", &mMaterial.shininess, 0.0f, 1.0f);
	ImGui::Checkbox("Visibility", &m_is_visible);

	SetTranslation(m_position);
	SetScale(m_scale);
	SetRotation(m_rotate);
}

