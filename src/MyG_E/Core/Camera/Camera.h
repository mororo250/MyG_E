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

	inline Vector<float, 3> get_position() const { return m_position; }
	inline void set_position(const Vector<float, 3> position) { m_position = position; }

	inline Vector<float, 3> get_direction() const { return m_direction; }
	void set_direction(Vector<float, 3> direction) { m_direction = direction; }

	inline float get_speed() const { return m_speed; }
	inline void set_speed(const float speed) { m_speed = speed; }

	inline float get_sensitivity() const { return m_sensitivity; }
	inline void set_sensitivity(const float sensitivity) { m_sensitivity = sensitivity; }

	virtual void imgui_renderer();
	virtual void update() = 0;

protected:
	virtual void translate() = 0;
	virtual void rotate() = 0;

	float m_speed; //speed of camera moviment
	float m_sensitivity; //velocity in witch the camera will change front_camera

	std::pair<float, float> m_mouse_pos; //currenty mouse possition

	Vector<float, 3> m_position;
	Vector<float, 3> m_direction;
	Matrix<float, 4, 4> m_view;
};
