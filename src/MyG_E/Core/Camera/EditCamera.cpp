// Header file.
#include "EditCamera.h"

// Interns 
#include "Foundation\Math\Quaternion.h"

// Third parties.
#include "imgui.h"


EditCamera::EditCamera(const Vector3f& position, const Vector3f& direction)
	: m_distance((position - direction).Length())
{
	m_position = position;
	m_direction = direction;
	Quaternion aux = get_orientation();
	m_view = TranslationMatrix4(-m_direction) * Quaternion::CreateRotationMatrix(aux) * TranslationMatrix4(-Vector3f({ 0.0f, 0.0f, m_distance }));
}

EditCamera::~EditCamera()
{
}

void EditCamera::update()
{
	translate();
	rotate();

	Quaternion aux = get_orientation();
	m_view = TranslationMatrix4(-m_direction) * Quaternion::CreateRotationMatrix(aux) * TranslationMatrix4(-Vector3f({0.0f, 0.0f, m_distance}));
	// Update position
	m_position = m_direction + Quaternion::rotate(aux, { 0.0f, 0.0f, m_distance });
}

void EditCamera::rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition());//get current mouse position
	
	if (Input::Get().IsMouseButtonPressed(MOUSE_MBUTTON))
	{
		// Get current up direction.
		Vector3f up = Quaternion::rotate(get_orientation(), { 0.0f, 1.0f, 0.0f });
		float pitch_sign = up[1] > 0 ? 1.0f : -1.0f;
		m_yaw += (m_mouse_pos.second - mouse_current_pos.second) * m_sensitivity;
		m_pitch += pitch_sign * (m_mouse_pos.first - mouse_current_pos.first) * m_sensitivity;
	}
	m_mouse_pos = mouse_current_pos;
}

void EditCamera::translate()
{
	m_distance = (m_position - m_direction).Length();
	float moved_distance = static_cast<float>(Input::Get().GetScrollOffset()) * m_speed / 10.0f * m_distance;
	if (Input::Get().GetScrollOffset() && moved_distance >= m_distance) 
		m_distance = m_distance * 0.2f;
	else
		m_distance -= moved_distance;
	Input::Get().SetScrollOffset(0.0);
}
