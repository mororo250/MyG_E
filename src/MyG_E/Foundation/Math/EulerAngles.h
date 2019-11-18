#pragma once
#include "Foundation/Math/Matrix.h"

class EulerAngles
{
public:
	explicit EulerAngles(float const yaw = 0.0f, float const pitch = 0.0f, float const roll = 0.0f);

	void set_yaw(float const angle);
	void set_pitch(float const angle);
	void set_roll(float const angle);
	void set_angles(Vector3f const& vec_angle);

	inline Matrix4x4f get_rotation_xyz() { return m_rot_x * m_rot_y * m_rot_z; }
	inline Matrix4x4f get_rotation_xzy() { return m_rot_x * m_rot_z * m_rot_y; }
	inline Matrix4x4f get_rotation_yxz() { return m_rot_y * m_rot_x * m_rot_z; }
	inline Matrix4x4f get_rotation_yzx() { return m_rot_y * m_rot_z * m_rot_x; }
	inline Matrix4x4f get_rotation_zxy() { return m_rot_z * m_rot_x * m_rot_y; }
	inline Matrix4x4f get_rotation_zyx() { return m_rot_z * m_rot_y * m_rot_x; }

	inline float get_yaw() { return m_yaw; }
	inline float get_pitch() { return m_pitch; }
	inline float get_roll() { return m_roll; }

private:
	float m_yaw;
	float m_pitch;
	float m_roll;

	Matrix4x4f m_rot_x;
	Matrix4x4f m_rot_y;
	Matrix4x4f m_rot_z;
};
