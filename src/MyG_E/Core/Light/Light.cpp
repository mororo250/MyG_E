#include "Light.h"
#include "imgui.h"

Light::Light(const Vector<float, 3>& position, const Vector<float, 3>& color)
	: m_color(color)
	, m_model(new Model3D(new Mesh(Shape::SPHERE)))
	, m_ambient_strength(0.1f)
	, m_diffuse_strength(1.0f)
	, m_specular_strength(1.0f)
{
	m_model->SetPosition(position);
	m_model->SetScale({0.01f, 0.01f, 0.01f});
}

Light::~Light()
{
	delete m_model;
}

void Light::ImGuiRenderer()
{
	Vector<float, 3> position = m_model->GetPosition();
	ImGui::DragFloat3("Translate", &position[0], 0.1f);
	m_model->SetPosition(position);
	ImGui::ColorEdit3("Light Color", &m_color[0]);
	ImGui::Separator();
	ImGui::DragFloat("Ambient stength", &m_ambient_strength, 0.05f , 0.0f, 1.0f);
	ImGui::DragFloat("Diffuse stength", &m_diffuse_strength, 0.05f ,0.0f, 5.0f);
	ImGui::DragFloat("Specular stength", &m_specular_strength, 0.05f, 0.0f, 5.0f);
	ImGui::Separator();
}
