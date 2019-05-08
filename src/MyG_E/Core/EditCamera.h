#pragma once

#include "Camera.h"

class EditCamera : public Camera
{
public:
	EditCamera(const Vector<float, 3>& position, const Vector<float, 3>& focal_point = {0.0f, 0.0f, 0.0f});
	~EditCamera();

	inline Vector<float, 3> GetFocalPoint() const { return m_focal_point; }
	void SetFocalPoint(const Vector<float, 3>& focal_point);

	void Update() override;

protected:
	void Rotate() override;
	void Translate() override;

private:
	Vector<float, 3> m_focal_point;
	float m_distance; // The distance between the focal point and the camera position.
	float m_yaw;
	float m_pitch;

	class Quaternion GetOrientation() const;
};

