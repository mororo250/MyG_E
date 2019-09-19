#include "SpotLight.h"
#include "Core\Shader.h"

#include <string>
#include "imgui.h"

unsigned short SpotLight::s_count = 0;

SpotLight::SpotLight(const Vector<float, 3>& position, const Vector<float, 3>& color, float in_angle, float out_angle, const Vector<float, 3>& direction)
	: m_in_angle(in_angle)
	, m_out_angle(out_angle)
	, m_direction(direction)
	, Light(position, color)
	, m_id(s_count++)
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

void SpotLight::SetUniform(Shader* shader)
{
	std::string light = "u_SpotLight[" + std::to_string(m_id) + "]";
	shader->SetUniform3f(shader->GetUniformLocation(light + ".light.color"), GetLightColor());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.ambient_strength"), GetAmbientStength());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.diffuse_strength"), GetDiffuseStrength());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.specular_strength"), GetSpecularStrength());
	shader->SetUniform3f(shader->GetUniformLocation(light + ".position"), GetLightPosition());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".in_angle"), std::cos(m_in_angle));
	shader->SetUniform1f(shader->GetUniformLocation(light + ".out_angle"), std::cos(m_out_angle));
	shader->SetUniform3f(shader->GetUniformLocation(light + ".direction"), m_direction);
}