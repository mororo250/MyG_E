#include "FPSCamera.h"
// Interns 
#include "Foundation\Math\Quaternion.h"


FPSCamera::FPSCamera(Vector3f position, Vector3f direction)
	: m_up({ 0.0f, 1.0f, 0.0f })
	, m_direction_begin(Vector3f(direction - position).Normalize())
{
	m_direction = m_direction_begin;
	m_position = position;
	m_view = LookAt(m_position, m_direction, m_up);
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::update()
{
	translate();
	rotate();
	Quaternion aux = get_orientation();
	m_direction = Quaternion::rotate(aux, m_direction_begin).Normalize();
	m_view = LookAt(m_position, m_direction + m_position, m_up);
}

void FPSCamera::rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition());//get current mouse position

	if (Input::Get().IsMouseButtonPressed(MOUSE_LBUTTON))
	{
		// Get current up direction.
		m_up = Quaternion::rotate(get_orientation(), { 0.0f, 1.0f, 0.0f });
		float pitch_sign = m_up[1] > 0 ? 1.0f : -1.0f;
		m_yaw += (m_mouse_pos.second - mouse_current_pos.second) * m_sensitivity;
		m_pitch += pitch_sign * (m_mouse_pos.first - mouse_current_pos.first) * m_sensitivity;
	}
	m_mouse_pos = mouse_current_pos;
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
