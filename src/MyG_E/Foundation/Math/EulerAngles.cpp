#include "EulerAngles.h"

EulerAngles::EulerAngles(float const yaw, float const pitch, float const roll)
	: m_rot_x(Matrix4x4f::make_rotation_matrix_x(yaw))
	, m_rot_y(Matrix4x4f::make_rotation_matrix_x(pitch))
	, m_rot_z(Matrix4x4f::make_rotation_matrix_x(roll))
	, m_yaw(yaw)
	, m_pitch(pitch)
	, m_roll(roll)
{}

void EulerAngles::set_yaw(float const angle)
{
	m_rot_x.set_angle_x(angle);
}

void EulerAngles::set_pitch(float const angle)
{
	m_rot_x.set_angle_y(angle);
}

void EulerAngles::set_roll(float const angle)
{
	m_rot_x.set_angle_z(angle);
}

void EulerAngles::set_angles(Vector3f const& vec_angle)
{
	m_rot_x.set_angle_x(vec_angle[0]);
	m_rot_y.set_angle_y(vec_angle[1]);
	m_rot_z.set_angle_z(vec_angle[2]);
}
