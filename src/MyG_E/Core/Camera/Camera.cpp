#include "Camera.h"

// Interns 
#include "Foundation\Math\Quaternion.h"

// Extern
#include "imgui.h"

Camera::Camera(float yaw, float pitch)
	: m_speed(0.5f)
	, m_sensitivity(0.005f)
	, m_mouse_pos(std::move(Input::Get().GetMousePosition()))
	, m_current_mouse_pos(m_mouse_pos)
	, m_yaw(yaw)
	, m_pitch(pitch)
{
}

Camera::~Camera()
{
}

void Camera::imgui_renderer()
{
	ImGui::Begin("Camera_infomation");
	ImGui::Text("Camera Position: X:%f Y:%f Z:%f", m_position[0], m_position[1], m_position[2]);
	ImGui::DragFloat("Speed: ", &m_speed, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("Sensitivity: ", &m_sensitivity, 0.001f ,0.000f, 0.010f);
	ImGui::Text("yaw: %f pitch:%f", m_yaw, m_pitch);
	ImGui::End();
}

Quaternion Camera::get_orientation() const
{
	Quaternion quatx = Quaternion::make_rotate(m_yaw, { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::make_rotate(m_pitch, { 0.0f, 1.0f, 0.0f });
	return quaty * quatx;
}