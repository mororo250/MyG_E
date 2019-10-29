#include "FPSCamera.h"
// Interns 
#include "Foundation\Math\Quaternion.h"

FPSCamera::FPSCamera(Vector3f const& position, float yaw, float pitch)
	:Camera(yaw, pitch)
{
	m_up = Quaternion::rotate(get_orientation(), Y_AXIS);
	m_position = position;
	m_direction = Quaternion::rotate(get_orientation(), DIRECTION_AUXILIAR).Normalize();
	m_view = LookAt(m_position, m_direction, m_up);
}

FPSCamera::FPSCamera(Camera const* other)
	:Camera(*other)
{
	m_up = Quaternion::rotate(get_orientation(), Y_AXIS);
	m_direction = Quaternion::rotate(get_orientation(), DIRECTION_AUXILIAR).Normalize();
	m_view = LookAt(m_position, m_direction + m_position, m_up);
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::update()
{
	m_current_mouse_pos = Input::Get().GetMousePosition(); // Get current mouse position.
	translate();
	rotate();
	m_mouse_pos = m_current_mouse_pos; // Update mouse Position.

	m_direction = Quaternion::rotate(get_orientation(), DIRECTION_AUXILIAR).Normalize();
	m_view = LookAt(m_position, m_direction + m_position, m_up);
}

void FPSCamera::rotate()
{
	if (Input::Get().IsMouseButtonPressed(MOUSE_LBUTTON))
	{
		// Get current up direction.
		m_up = Quaternion::rotate(get_orientation(), Y_AXIS);
		float pitch_sign = m_up[1] > 0 ? 1.0f : -1.0f;
		m_yaw += (m_mouse_pos.second - m_current_mouse_pos.second) * m_sensitivity;
		m_pitch += pitch_sign * (m_mouse_pos.first - m_current_mouse_pos.first) * m_sensitivity;
	}
}

void FPSCamera::translate()
{
	if (Input::Get().IsKeyPressed(KEY_W))
		m_position += m_speed * m_direction;
	if (Input::Get().IsKeyPressed(KEY_S))
		m_position -= m_speed * m_direction;
	if (Input::Get().IsKeyPressed(KEY_A))
		m_position -= m_speed * Vector3f::Cross(m_direction, m_up);
	if (Input::Get().IsKeyPressed(KEY_D))
		m_position += m_speed * Vector3f::Cross(m_direction, m_up);
}
