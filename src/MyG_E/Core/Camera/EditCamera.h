#pragma once

#include "Camera.h"

class EditCamera : public Camera
{
public:
	EditCamera(const Vector<float, 3>& position, const Vector<float, 3>& direction = {0.0f, 0.0f, 0.0f});
	~EditCamera();

	void update() override;

protected:
	void rotate() override;
	void translate() override;

private:
	float m_distance; // The distance between the focal point and the camera position.
	float m_yaw;
	float m_pitch;

	class Quaternion get_orientation() const;
};

