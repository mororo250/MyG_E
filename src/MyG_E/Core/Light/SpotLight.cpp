#include "SpotLight.h"
#include "imgui.h"

SpotLight::SpotLight(const Vector<float, 3>& position, const Vector<float, 3>& color, float in_angle, float out_angle, const Vector<float, 3>& direction)
	: m_in_angle(in_angle)
	, m_out_angle(out_angle)
	, m_direction(direction)
	, Light(position, color)
{
}

void SpotLight::ImGuiRenderer()
{
	Light::ImGuiRenderer();
	ImGui::SliderAngle("In angle:", &m_in_angle, 0.0f, 120.0f);
	ImGui::SliderAngle("Out angle:", &m_out_angle, 0.0f, 120.0f);
	if (m_out_angle < m_in_angle)
		m_out_angle = m_in_angle;
}
