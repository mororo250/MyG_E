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

void Camera::imgui_renderer()
{
	ImGui::Begin("Camera_infomation");
	ImGui::Text("Camera Position: X:%f Y:%f Z:%f", m_position[0], m_position[1], m_position[2]);
	ImGui::DragFloat("Speed: ", &m_speed, 0.0f, 10.0f);
	ImGui::DragFloat("Sensitivity: ", &m_sensitivity, 0.000f, 0.010f);
	ImGui::End();
}
