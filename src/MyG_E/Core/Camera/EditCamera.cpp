// Header file.
#include "EditCamera.h"

// Interns 
#include "Foundation\Math\Quaternion.h"

// Third parties.
#include "imgui.h"


EditCamera::EditCamera(Vector3f const& position, Vector3f const& focal_point, float yaw, float pitch)
	: Camera(yaw, pitch)
	, m_distance((position - focal_point).Length())
	, m_focal_point(focal_point)
{
	update_view();
}

EditCamera::~EditCamera()
{
}

void EditCamera::update()
{
	m_current_mouse_pos = Input::Get().GetMousePosition(); // Get current mouse position.
	translate();
	rotate();
	m_mouse_pos = m_current_mouse_pos; // Update mouse Position.

	update_view();
}

void EditCamera::rotate()
{
	if (Input::Get().IsMouseButtonPressed(MOUSE_MBUTTON) && !Input::Get().IsKeyPressed(KEY_LSHIFT))
	{
		// Get current up direction.
		Vector3f up = Quaternion::rotate(get_orientation(), Y_AXIS);
		float pitch_sign = up[1] > 0 ? 1.0f : -1.0f;
		m_yaw += (m_mouse_pos.second - m_current_mouse_pos.second) * m_sensitivity;
		m_pitch += pitch_sign * (m_mouse_pos.first - m_current_mouse_pos.first) * m_sensitivity;
	}
}

void EditCamera::translate()
{
	m_distance = (m_position - m_focal_point).Length();
	float moved_distance = static_cast<float>(Input::Get().GetScrollOffset()) * m_speed / 10.0f * m_distance;
	if (Input::Get().GetScrollOffset() && moved_distance >= m_distance) 
		m_distance = m_distance * 0.2f;
	else
		m_distance -= moved_distance;
	Input::Get().SetScrollOffset(0.0);

	if (Input::Get().IsKeyPressed(KEY_LSHIFT) && Input::Get().IsMouseButtonPressed(MOUSE_MBUTTON))
	{
		Vector3f offset(m_mouse_pos.first - m_current_mouse_pos.first, m_current_mouse_pos.second - m_mouse_pos.second, 0.0f);
		offset *= m_speed * 0.1f;
		offset = Quaternion::rotate(get_orientation(), offset);
		m_focal_point += offset;
		m_position += offset;
	}
}

void EditCamera::update_view()
{
	Quaternion quat = get_orientation();
	Vector3f vec(0.0f, 0.0f, m_distance);
	m_view = TranslationMatrix4(-m_focal_point) * Quaternion::CreateRotationMatrix(quat) * TranslationMatrix4(-vec);
	m_position = m_focal_point + Quaternion::rotate(quat, vec);
}
