// Header file.
#include "EditCamera.h"

// Interns 
#include "Foundation\Math\Quaternion.h"

// Third parties.
#include "imgui.h"


EditCamera::EditCamera(const Vector<float, 3>& position, const Vector<float, 3>& focal_point)
	: m_distance((position - focal_point).Length())
	, m_yaw(0.0f)
	, m_pitch(0.0f)
{
	m_position = position;
	m_focal_point = focal_point;
	m_view = LookAt(m_position, m_focal_point, {0.0f, 1.0f, 0.0f});
}

EditCamera::~EditCamera()
{
}

void EditCamera::SetFocalPoint(const Vector<float, 3>& focal_point) 
{
	m_focal_point = focal_point;
}

void EditCamera::Update()
{
	Translate();
	Rotate();

	Quaternion aux = GetOrientation();
	m_view = TranslationMatrix4(-m_focal_point) * Quaternion::CreateRotationMatrix(aux) * TranslationMatrix4(-Vector<float, 3>({0.0f, 0.0f, m_distance}));
	// Update position
	m_position = m_focal_point + Quaternion::rotate(aux, { 0.0f, 0.0f, m_distance });
}

void EditCamera::Rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition());//get current mouse position
	
	if (Input::Get().IsMouseButtonPressed(MOUSE_MBUTTON))
	{
		// Get current up direction.
		Vector<float, 3> up = Quaternion::rotate(GetOrientation(), { 0.0f, 1.0f, 0.0f });
		float pitch_sign = up[1] > 0 ? 1.0f : -1.0f;
		m_yaw += (m_mouse_pos.second - mouse_current_pos.second) * m_sensitivity;
		m_pitch += pitch_sign * (m_mouse_pos.first - mouse_current_pos.first) * m_sensitivity;
	}
	m_mouse_pos = mouse_current_pos;
}

void EditCamera::Translate()
{
	m_distance = (m_position - m_focal_point).Length();
	float moved_distance = Input::Get().GetScrollOffset() * m_speed / 10.0f * m_distance;
	if (Input::Get().GetScrollOffset() && moved_distance >= m_distance) 
		m_distance = m_distance * 0.2f;
	else
		m_distance -= moved_distance;
	Input::Get().SetScrollOffset(0.0);
}

Quaternion EditCamera::GetOrientation() const
{
	Quaternion quatx = Quaternion::MakeRotate(m_yaw, { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::MakeRotate(m_pitch, { 0.0f, 1.0f, 0.0f });
	return quaty * quatx;
}
