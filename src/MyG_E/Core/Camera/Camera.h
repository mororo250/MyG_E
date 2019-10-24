#pragma once

#include "Foundation/Input.h"
#include "Foundation/Math/Vector.h"
#include "Foundation/Math/Matrix.h"

class Camera
{
public:
	Camera(float speed = 0.5f, float sensitivity = 0.005f);
	virtual ~Camera();

	inline Matrix<float, 4, 4> & get_view() { return m_view; }

	inline Vector3f get_position() const { return m_position; }
	inline void set_position(const Vector3f position) { m_position = position; }

	inline Vector3f get_direction() const { return m_direction; }
	void set_direction(Vector3f direction) { m_direction = direction; }

	inline float get_speed() const { return m_speed; }
	inline void set_speed(const float speed) { m_speed = speed; }

	inline float get_sensitivity() const { return m_sensitivity; }
	inline void set_sensitivity(const float sensitivity) { m_sensitivity = sensitivity; }

	virtual void imgui_renderer();
	virtual void update() = 0;

protected:
	virtual void translate() = 0;
	virtual void rotate() = 0;
	class Quaternion get_orientation() const;

	float m_speed; //speed of camera moviment
	float m_sensitivity; //velocity in witch the camera will change front_camera

	std::pair<float, float> m_mouse_pos; //currenty mouse possition

	Vector3f m_position;
	Vector3f m_direction;
	Matrix<float, 4, 4> m_view;

	float m_yaw;
	float m_pitch;
};
