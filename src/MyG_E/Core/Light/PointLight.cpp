#include "PointLight.h"
#include "Core\Shader.h"

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
	ImGui::DragFloat("Constant:", &m_attenuation_constants[0], 0.05, 0.0f, 1.0f);
	ImGui::DragFloat("Linear:", &m_attenuation_constants[1], 0.05f, 0.0f, 1.0f);
	ImGui::DragFloat("Quadratic:", &m_attenuation_constants[2], 0.05f, 0.0f, 1.0f);
}

void PointLight::set_uniform(Shader* shader)
{
	std::string light = "u_PointLight[" + std::to_string(m_id) + "]";
	shader->set_uniform3f(shader->GetUniformLocation(light + ".light.color"), get_light_color());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".light.ambient_strength"), get_ambient_strength());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".light.diffuse_strength"), get_diffuse_strength());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".light.specular_strength"), get_specular_strength());
	shader->set_uniform3f(shader->GetUniformLocation(light + ".position"), get_light_position());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".constant"), GetConstant());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".linear"), GetLinear());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".quadratic"), GetQuadratic());
}
