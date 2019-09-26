#include "Light.h"
#include "imgui.h"

Light::Light(const Vector<float, 3>& position, const Vector<float, 3>& color)
	: m_color(color)
	, m_model(new Model3D(new Mesh(Shape::SPHERE)))
	, m_ambient_strength(0.1f)
	, m_diffuse_strength(1.0f)
	, m_specular_strength(1.0f)
{
	m_model->set_position(position);
	m_model->set_scale({0.01f, 0.01f, 0.01f});
}

Light::Light(Light const& other)
{
	copy_other(other);
}

Light& Light::operator=(Light const& other)
{
	delete m_model;

	copy_other(other);
	return *this;
}

Light::~Light()
{
	delete m_model;
}

void Light::ImGuiRenderer()
{
	Vector<float, 3> position = m_model->get_position();
	ImGui::DragFloat3("Translate", &position[0], 0.1f);
	m_model->set_position(position);
	ImGui::ColorEdit3("Light Color", &m_color[0]);
	ImGui::Separator();
	ImGui::DragFloat("Ambient stength", &m_ambient_strength, 0.05f , 0.0f, 1.0f);
	ImGui::DragFloat("Diffuse stength", &m_diffuse_strength, 0.05f ,0.0f, 5.0f);
	ImGui::DragFloat("Specular stength", &m_specular_strength, 0.05f, 0.0f, 5.0f);
	ImGui::Separator();
}

void Light::copy_other(Light const& other)
{
	m_color = other.m_color;
	m_ambient_strength = other.m_ambient_strength;
	m_diffuse_strength = other.m_diffuse_strength;
	m_specular_strength = other.m_specular_strength;
	m_model = new Model3D(*m_model);
}
