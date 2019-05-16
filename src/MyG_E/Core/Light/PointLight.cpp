#include "PointLight.h"
#include "imgui.h"

PointLight::PointLight(const Vector<float, 3>& position, const Vector<float, 3>& color)
	: m_attenuation_constants({ 1.0f, 0.09f, 0.032f }) // Light cover 50m. 
	, Light(position, color)
{
}

void PointLight::ImGuiRenderer()
{
	Light::ImGuiRenderer();
	ImGui::SliderFloat("Constant:", &m_attenuation_constants[0], 0.0f, 1.0f);
	ImGui::SliderFloat("Linear:", &m_attenuation_constants[1], 0.0f, 1.0f);
	ImGui::SliderFloat("Quadratic:", &m_attenuation_constants[2], 0.0f, 1.0f);
}
