#include "DirectionalLight.h"
#include "imgui.h"

DirectionalLight::DirectionalLight(const Vector<float, 3>& position, const Vector<float, 3>& color, const Vector<float, 3>& directional)
	: m_directional(directional)
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