#include "AmbientLight.h"

#include "imgui.h"

AmbientLight::AmbientLight(float const strength, Vector3f const& color)
	: m_color(color)
	, m_strength(strength)
{
}

void AmbientLight::imgui_renderer()
{
	ImGui::ColorEdit3("Light Color", m_color.get_as_pointer());
	ImGui::DragFloat("stength", &m_strength, 0.05f, 0.0f, 5.0f);
}
