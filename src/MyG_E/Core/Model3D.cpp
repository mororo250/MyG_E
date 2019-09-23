// Heap file.
#include "Model3D.h"

// Intern files.
#include "Foundation\Math\Quaternion.h"

// std fiels
#include <string>

// Third Parties.
#include "imgui.h"

unsigned int Model3D::m_number_of_objects = 0;

Model3D::Model3D(Mesh* mesh)
	: m_position({ 0.0f, 0.0f, 0.0f })
	, m_trans_matrix(0.0f, 0.0f, 0.0f)
	, m_scale_matrix(1.0f, 1.0f, 1.0f)
	, m_scale({ 1.0f, 1.0f, 1.0f })
	, m_rotation_matrix(Matrix<float, 4, 4>::make_identity())
	, m_mesh(new Mesh(*mesh))
	, m_is_visible(true)
{
	m_object_name = "Object " + std::to_string(m_number_of_objects);
	m_number_of_objects++;
}

Model3D::Model3D(Mesh* mesh, std::string& const name)
	: m_position({ 0.0f, 0.0f, 0.0f }),
	m_trans_matrix(0.0f, 0.0f, 0.0f),
	m_scale_matrix(1.0f, 1.0f, 1.0f),
	m_scale({1.0f, 1.0f, 1.0f}),
	m_rotation_matrix(),
	m_mesh(new Mesh(*mesh)),
	m_object_name(name),
	m_is_visible(true)
{
	m_number_of_objects++;
}

Model3D::Model3D(Model3D& const model3d)
	: m_trans_matrix(0.0f, 0.0f, 0.0f),
	m_scale_matrix(1.0f, 1.0f, 1.0f),
	m_rotation_matrix()
{
	m_mesh = new Mesh(*model3d.m_mesh);
	mMaterial = model3d.mMaterial;

	SetTranslation(model3d.m_position);
	SetScale(model3d.m_scale);
	SetRotation(model3d.m_rotate);

	m_object_name = model3d.m_object_name;
	m_is_visible = model3d.m_is_visible;
	
	m_number_of_objects = model3d.m_number_of_objects;
}

Model3D::~Model3D()
{
	delete m_mesh;
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
	ImGui::DragFloat3("Translate", &m_position[0], 0.1f);
	ImGui::DragFloat3("Scale", &m_scale[0], 0.1f, 0.0f, 100.0f);
	ImGui::SliderFloat3("Rotate", &m_rotate[0], -6.28f, 6.28f);
	ImGui::ColorEdit3("Ambient", &mMaterial.ambient[0]);
	ImGui::ColorEdit3("Diffuse", &mMaterial.diffuse[0]);
	ImGui::ColorEdit3("Specular", &mMaterial.specular[0]);
	ImGui::DragFloat("shininess", &mMaterial.shininess, 0.05f, 0.0f, 1.0f);
	ImGui::Checkbox("Visibility", &m_is_visible);

	SetTranslation(m_position);
	SetScale(m_scale);
	SetRotation(m_rotate);
}

