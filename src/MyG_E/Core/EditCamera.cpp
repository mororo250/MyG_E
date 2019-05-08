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
	mPosition = position;
	m_focal_point = focal_point;
	mView = LookAt(mPosition, m_focal_point, {0.0f, 1.0f, 0.0f});
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
	mView = TranslationMatrix4(-m_focal_point) * Quaternion::create_rotation_matrix(aux) * TranslationMatrix4(-Vector<float, 3>({0.0f, 0.0f, m_distance}));
	// Update position
	mPosition = m_focal_point + Quaternion::rotate(aux, { 0.0f, 0.0f, m_distance });
}

void EditCamera::Rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition());//get current mouse position
	
	if (Input::Get().IsMouseButtonPressed(MOUSE_MBUTTON))
	{
		// Get current up direction.
		Vector<float, 3> up = Quaternion::rotate(GetOrientation(), { 0.0f, 1.0f, 0.0f });
		float pitch_sign = up[1] > 0 ? 1.0f : -1.0f;
		m_yaw += (mMousePos.second - mouse_current_pos.second) * mSensitivity;
		m_pitch += pitch_sign * (mMousePos.first - mouse_current_pos.first) * mSensitivity;
	}
	mMousePos = mouse_current_pos;
}

void EditCamera::Translate()
{
	m_distance = (mPosition - m_focal_point).Length();
	float moved_distance = Input::Get().GetScrollOffset() * mSpeed / 10.0f * m_distance;
	if (Input::Get().GetScrollOffset() && moved_distance >= m_distance) 
		m_distance = m_distance * 0.2f;
	else
		m_distance -= moved_distance;
	Input::Get().SetScrollOffset(0.0);
}

Quaternion EditCamera::GetOrientation() const
{
	Quaternion quatx = Quaternion::make_rotate(m_yaw, { 1.0f, 0.0f, 0.0f });
	Quaternion quaty = Quaternion::make_rotate(m_pitch, { 0.0f, 1.0f, 0.0f });
	return quaty * quatx;
}
