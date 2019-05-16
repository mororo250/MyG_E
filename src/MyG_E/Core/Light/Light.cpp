#include "Light.h"
#include "imgui.h"

Light::Light(const Vector<float, 3>& position, const Vector<float, 3>& color)
	: m_color(color)
	, m_model(std::make_shared<Mesh>(Shape::SPHERE))
	, m_ambient_strength(0.1f)
	, m_diffuse_strength(1.0f)
	, m_specular_strength(1.0f)
{
	m_model.SetPosition(position);
	m_model.SetScale({0.01f, 0.01f, 0.01});
}

void Light::ImGuiRenderer()
{
	ImGui::ColorEdit3("Light Color", &m_color[0]);
	ImGui::SliderFloat("Ambient stength:", &m_ambient_strength, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse stength:", &m_diffuse_strength, 0.0f, 5.0f);
	ImGui::SliderFloat("Specular stength:", &m_specular_strength, 0.0f, 5.0f);
}
