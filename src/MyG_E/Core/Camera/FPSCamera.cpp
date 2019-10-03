#include "FPSCamera.h"

FPSCamera::FPSCamera(Vector<float, 3> position, Vector<float, 3> direction)
{
	m_position = position;
	m_direction = direction;
	m_view = LookAt(m_position, m_direction, { 0.0f, 1.0f, 0.0f });
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::update()
{
	translate();
	rotate();

	m_view = LookAt(m_position, (m_position + m_direction), { 0.0f, 1.0f, 0.0f });
}

void FPSCamera::rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition()); //get current mouse position

	RotationMatrix3 pitch = RotationMatrix3((mouse_current_pos.second - m_mouse_pos.second) * m_sensitivity, AxisUsage::AXIS_X); // x axis rotation
	RotationMatrix3 yaw = RotationMatrix3((mouse_current_pos.first - m_mouse_pos.first) * m_sensitivity, AxisUsage::AXIS_Y); //Y axis rotation
	
	m_direction = m_direction * yaw * pitch;
	m_mouse_pos = mouse_current_pos;
}

void FPSCamera::translate()
{
	if (Input::Get().IsKeyPressed(KEY_W))
		m_position += m_speed * m_direction;
	if (Input::Get().IsKeyPressed(KEY_S))
		m_position -= m_speed * m_direction;
	if (Input::Get().IsKeyPressed(KEY_A))
		m_position -= m_speed * Cross(m_direction, { 0.0f, 1.0f, 0.0f });
	if (Input::Get().IsKeyPressed(KEY_D))
		m_position += m_speed * Cross(m_direction, { 0.0f, 1.0f, 0.0f });
}
