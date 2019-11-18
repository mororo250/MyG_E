#pragma once

#include "Foundation/Input.h"
#include "Foundation/Math/Vector.h"
#include "Foundation/Math/Matrix.h"

class Camera
{
public:
	Camera(float yaw = 0.0f, float pitch = 0.0f);
	virtual ~Camera();

	inline Matrix<float, 4, 4> &get_view() { return m_view; }

	inline Vector3f get_position() const { return m_position; }
	inline void set_position(const Vector3f position) { m_position = position; }

	inline float get_speed() const { return m_speed; }
	inline void set_speed(const float speed) { m_speed = speed; }

	inline float get_sensitivity() const { return m_sensitivity; }
	inline void set_sensitivity(const float sensitivity) { m_sensitivity = sensitivity; }

	inline float get_yaw() const { return m_yaw; }
	inline float get_pitch() const { return m_pitch; }

	virtual void imgui_renderer();
	virtual void update() = 0;
	virtual Vector3f get_direction() const = 0;

protected:
	virtual void translate() = 0;
	virtual void rotate() = 0;
	class Quaternion get_orientation() const;

	float m_speed; //speed of camera moviment
	float m_sensitivity; //velocity in witch the camera will change front_camera

	std::pair<float, float> m_mouse_pos; // Last mouse possition.
	std::pair<float, float> m_current_mouse_pos; // Current frame mouse possition.

	Vector3f m_position;
	Matrix<float, 4, 4> m_view;

	float m_yaw;
	float m_pitch;

	static constexpr Vector3f DIRECTION_AUXILIAR = Vector3f(0.0f, 0.0f, -1.0f);
};
