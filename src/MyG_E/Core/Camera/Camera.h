#pragma once

#include "Foundation/Input.h"
#include "Foundation/Math/Vector.h"
#include "Foundation/Math/Matrix.h"

class Camera 
{
public:
	Camera(float speed = 0.5f, float sensitivity = 0.005f);
	virtual ~Camera();

	inline Matrix<float, 4, 4>& GetView() { return m_view; }

	inline Vector<float, 3> GetPosition() const { return m_position; }
	inline void SetPosition(const Vector<float, 3> position) { m_position = position; }
	
	inline float GetSpeed() const { return m_speed; }
	inline void SetSpeed(const float speed) { m_speed = speed; }
	
	inline float GetSensitivity() const { return m_sensitivity; }
	inline void SetSensitivity(const float sensitivity) { m_sensitivity = sensitivity; }

	virtual void ImGuiRenderer();
	virtual void Update() = 0;

protected:
	virtual void Translate() = 0;
	virtual void Rotate() = 0;

	float m_speed; //speed of camera moviment
	float m_sensitivity; //velocity in witch the camera will change front_camera

	std::pair<float, float> m_mouse_pos; //currenty mouse possition

	Vector<float, 3> m_position;
	Matrix<float, 4, 4> m_view;
};
