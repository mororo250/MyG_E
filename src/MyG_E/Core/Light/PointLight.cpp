#include "PointLight.h"
#include "Core\Shader.h"

#include <string>
#include "imgui.h"

unsigned short PointLight::s_count = 0;

PointLight::PointLight(const Vector<float, 3>& position, const Vector<float, 3>& color)
	: m_attenuation_constants({ 1.0f, 0.09f, 0.032f }) // Light cover 50m. 
	, Light(position, color)
	, m_id(s_count++)
{
}

void PointLight::ImGuiRenderer()
{
	Light::ImGuiRenderer();
	ImGui::SliderFloat("Constant:", &m_attenuation_constants[0], 0.0f, 1.0f);
	ImGui::SliderFloat("Linear:", &m_attenuation_constants[1], 0.0f, 1.0f);
	ImGui::SliderFloat("Quadratic:", &m_attenuation_constants[2], 0.0f, 1.0f);
}

void PointLight::SetUniform(Shader* shader)
{
	std::string light = "u_PointLight[" + std::to_string(m_id) + "]";
	shader->SetUniform3f(shader->GetUniformLocation(light + ".light.color"), GetLightColor());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.ambient_strength"), GetAmbientStength());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.diffuse_strength"), GetDiffuseStrength());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.specular_strength"), GetSpecularStrength());
	shader->SetUniform3f(shader->GetUniformLocation(light + ".position"), GetLightPosition());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".constant"), GetConstant());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".linear"), GetLinear());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".quadratic"), GetQuadratic());
}
