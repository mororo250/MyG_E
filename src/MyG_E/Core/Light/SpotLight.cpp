#include "SpotLight.h"
#include "Core\Shader.h"

#include "imgui.h"

unsigned short SpotLight::s_count = 0;
unsigned short SpotLight::s_id = 0;

SpotLight::SpotLight(const Vector3f& position, const Vector3f& color, float in_angle, float out_angle, const Vector3f& direction)
	: m_in_angle(in_angle)
	, m_out_angle(out_angle)
	, m_direction(direction)
	, Light(position, color)
{
	s_count++;
}

void SpotLight::ImGuiRenderer()
{
	Light::ImGuiRenderer();
	ImGui::SliderAngle("In angle:", &m_in_angle, 0.0f, 120.0f);
	ImGui::SliderAngle("Out angle:", &m_out_angle, 0.0f, 120.0f);
	if (m_out_angle < m_in_angle)
		m_out_angle = m_in_angle;
}

void SpotLight::set_uniform(Shader const* shader)
{
	std::string light = "u_SpotLight[" + std::to_string(s_id++) + "]";
	set_general_uniform(shader, light);
	shader->set_uniform3f(shader->get_uniform_location(light + ".position"), get_light_position());
	shader->set_uniform1f(shader->get_uniform_location(light + ".in_angle"), std::cos(m_in_angle));
	shader->set_uniform1f(shader->get_uniform_location(light + ".out_angle"), std::cos(m_out_angle));
	shader->set_uniform3f(shader->get_uniform_location(light + ".direction"), m_direction);
	if (s_id >= s_count)
		s_id = 0;
}
