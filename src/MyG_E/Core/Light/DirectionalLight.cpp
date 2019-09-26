#include "DirectionalLight.h"
#include "Core\Shader.h"

#include "imgui.h"

unsigned short DirectionalLight::s_count = 0;

DirectionalLight::DirectionalLight(const Vector<float, 3>& position, const Vector<float, 3>& color, const Vector<float, 3>& direction)
	: m_direction(direction)
	, m_id(s_count++)
	, Light(position, color)
{
}

void DirectionalLight::ImGuiRenderer()
{
	Light::ImGuiRenderer();
	ImGui::DragFloat3("Direction", &m_direction[0], 0.05f, 0.0f, 1.0f);
	m_direction.Normalize();
}

void DirectionalLight::set_uniform(Shader* shader)
{
	std::string light = "u_DirectionalLight[" + std::to_string(m_id) + "]";
	shader->set_uniform3f(shader->GetUniformLocation(light + ".light.color"), GetLightColor());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".light.ambient_strength"), GetAmbientStength());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".light.diffuse_strength"), GetDiffuseStrength());
	shader->set_uniform1f(shader->GetUniformLocation(light + ".light.specular_strength"), GetSpecularStrength());
	shader->set_uniform3f(shader->GetUniformLocation(light + ".directional"), m_direction);
}
