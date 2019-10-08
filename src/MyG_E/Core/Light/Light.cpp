#include "Light.h"
#include "imgui.h"
#include "Core/Shader.h"

Light::Light(const Vector3f& position, const Vector3f& color)
	: m_color(color)
	, m_model(new Model3D(Model3D::load_model("..\\..\\..\\Resources\\basic_meshes\\sphere.obj")))
	, m_ambient_strength(0.1f)
	, m_diffuse_strength(1.0f)
	, m_specular_strength(1.0f)
{
	m_model->set_position(position);
	m_model->set_scale({ 0.2f, 0.2f, 0.2f });
}

Light::Light(Light const& other)
{
	copy_other(other);
}

Light& Light::operator=(Light const& other)
{
	if (this != &other)
	{
		delete m_model;
		copy_other(other);
	}
	return *this;
}

Light::~Light()
{
	delete m_model;
}

void Light::ImGuiRenderer()
{
	Vector3f position = m_model->get_position();
	ImGui::DragFloat3("Translate", &position[0], 0.1f);
	m_model->set_position(position);
	ImGui::ColorEdit3("Light Color", &m_color[0]);
	ImGui::Separator();
	ImGui::DragFloat("Ambient stength", &m_ambient_strength, 0.05f , 0.0f, 1.0f);
	ImGui::DragFloat("Diffuse stength", &m_diffuse_strength, 0.05f ,0.0f, 5.0f);
	ImGui::DragFloat("Specular stength", &m_specular_strength, 0.05f, 0.0f, 5.0f);
	ImGui::Separator();
}

void Light::set_general_uniform(Shader const* shader, std::string const& uniform_name)
{
	shader->set_uniform3f(shader->get_uniform_location(uniform_name + ".light.color"), get_light_color());
	shader->set_uniform1f(shader->get_uniform_location(uniform_name + ".light.ambient_strength"), get_ambient_strength());
	shader->set_uniform1f(shader->get_uniform_location(uniform_name + ".light.diffuse_strength"), get_diffuse_strength());
	shader->set_uniform1f(shader->get_uniform_location(uniform_name + ".light.specular_strength"), get_specular_strength());
}

void Light::copy_other(Light const& other)
{
	m_color = other.m_color;
	m_ambient_strength = other.m_ambient_strength;
	m_diffuse_strength = other.m_diffuse_strength;
	m_specular_strength = other.m_specular_strength;
	m_model = new Model3D(*m_model);
}
