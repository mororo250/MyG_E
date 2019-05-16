#include "FPSCamera.h"

FPSCamera::FPSCamera(Vector<float, 3> position, Vector<float, 3> front_camera)
{
	m_position = position;
	m_front_camera = front_camera;
	m_view = LookAt(m_position, m_front_camera, { 0.0f, 1.0f, 0.0f });
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Update()
{
	Translate();
	Rotate();

	m_view = LookAt(m_position, (m_position + m_front_camera), { 0.0f, 1.0f, 0.0f });
}

void FPSCamera::Rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition()); //get current mouse position

	RotationMatrix3 pitch = RotationMatrix3((mouse_current_pos.second - m_mouse_pos.second) * m_sensitivity, AxisUsage::AXIS_X); // x axis rotation
	RotationMatrix3 yaw = RotationMatrix3((mouse_current_pos.first - m_mouse_pos.first) * m_sensitivity, AxisUsage::AXIS_Y); //Y axis rotation
	
	m_front_camera = m_front_camera * yaw * pitch;
	m_mouse_pos = mouse_current_pos;
}

void FPSCamera::Translate()
{
	if (Input::Get().IsKeyPressed(KEY_W))
		m_position += m_speed * m_front_camera;
	if (Input::Get().IsKeyPressed(KEY_S))
		m_position -= m_speed * m_front_camera;
	if (Input::Get().IsKeyPressed(KEY_A))
		m_position -= m_speed * Cross(m_front_camera, { 0.0f, 1.0f, 0.0f });
	if (Input::Get().IsKeyPressed(KEY_D))
		m_position += m_speed * Cross(m_front_camera, { 0.0f, 1.0f, 0.0f });

}
