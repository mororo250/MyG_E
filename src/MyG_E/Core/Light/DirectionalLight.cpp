#include "DirectionalLight.h"
#include "Core\Shader.h"

#include "imgui.h"

unsigned short DirectionalLight::s_count = 0;
unsigned short DirectionalLight::s_id = 0;

DirectionalLight::DirectionalLight(const Vector3f& position, const Vector3f& color, const Vector3f& direction)
	: m_direction(direction)
	, Light(position, color)
{
	s_count++;
}

void DirectionalLight::ImGuiRenderer()
{
	Light::ImGuiRenderer();
	ImGui::DragFloat3("Direction", &m_direction[0], 0.05f, 0.0f, 1.0f);
	m_direction.Normalize();
}

void DirectionalLight::set_uniform(Shader const* shader)
{
	std::string light = "u_DirectionalLight[" + std::to_string(s_id++) + "]";
	set_general_uniform(shader, light);
	shader->set_uniform3f(shader->get_uniform_location(light + ".directional"), m_direction);
	if (s_id >= s_count)
		s_id = 0;
}
