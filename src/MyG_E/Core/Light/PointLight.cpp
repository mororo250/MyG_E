#include "PointLight.h"
#include "Core\Shader.h"

#include "imgui.h"

unsigned short PointLight::s_count = 0;
unsigned short PointLight::s_id = 0;

PointLight::PointLight(const Vector3f& position, const Vector3f& color)
	: m_attenuation_constants({ 1.0f, 0.09f, 0.032f }) // Light cover 50m. 
	, Light(position, color)
{
	s_count++;
}

void PointLight::ImGuiRenderer()
{
	Light::ImGuiRenderer();
	if (ImGui::CollapsingHeader("Attenuation", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Constant:", &m_attenuation_constants[0], 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Linear:", &m_attenuation_constants[1], 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Quadratic:", &m_attenuation_constants[2], 0.05f, 0.0f, 1.0f);
	}
}

void PointLight::set_uniform(Shader const* shader)
{
	std::string light = "u_PointLight[" + std::to_string(s_id++) + "]";
	set_general_uniform(shader, light);
	shader->set_uniform3f(shader->get_uniform_location(light + ".position"), get_light_position());
	shader->set_uniform1f(shader->get_uniform_location(light + ".constant"), GetConstant());
	shader->set_uniform1f(shader->get_uniform_location(light + ".linear"), GetLinear());
	shader->set_uniform1f(shader->get_uniform_location(light + ".quadratic"), GetQuadratic());
	if (s_id >= s_count)
		s_id = 0;
}
