#include "DirectionalLight.h"
#include "Core\Shader.h"

#include <string>
#include "imgui.h"

unsigned short DirectionalLight::s_count = 0;

DirectionalLight::DirectionalLight(const Vector<float, 3>& position, const Vector<float, 3>& color, const Vector<float, 3>& directional)
	: m_directional(directional)
	, m_id(s_count++)
	, Light(position, color)
{
}

void DirectionalLight::ImGuiRenderer()
{
	static Vector<float, 3> directional = m_directional;
	Light::ImGuiRenderer();
	ImGui::SliderFloat3("Direction", &directional[0], 0.0f, 1.0f);
	m_directional = directional;
	m_directional.Normalize();
}

void DirectionalLight::SetUniform(Shader* shader)
{
	std::string light = "u_DirectionalLight[" + std::to_string(m_id) + "]";
	shader->SetUniform3f(shader->GetUniformLocation(light + ".light.color"), GetLightColor());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.ambient_strength"), GetAmbientStength());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.diffuse_strength"), GetDiffuseStrength());
	shader->SetUniform1f(shader->GetUniformLocation(light + ".light.specular_strength"), GetSpecularStrength());
	shader->SetUniform3f(shader->GetUniformLocation(light + ".directional"), m_directional);
}