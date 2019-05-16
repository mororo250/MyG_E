#include "Camera.h"
#include "imgui.h"

Camera::Camera(float speed, float sensitivity)
	:m_speed(speed),
	m_sensitivity(sensitivity),
	m_mouse_pos(std::move(Input::Get().GetMousePosition()))
{
}

Camera::~Camera()
{
}

void Camera::ImGuiRenderer()
{
	ImGui::Text("Mouse Position: %f %f", m_mouse_pos.first, m_mouse_pos.second);
	ImGui::SliderFloat("Speed: ", &m_speed, 0.0f, 10.0f);
	ImGui::SliderFloat("Sensitivity: ", &m_sensitivity, 0.010, 0.000);
}
