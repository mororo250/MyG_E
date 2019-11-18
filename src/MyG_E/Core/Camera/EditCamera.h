#pragma once

#include "Camera.h"

class EditCamera : public Camera
{
public:
	EditCamera(Vector3f const& position, Vector3f const& focal_point = Vector3f(0.0f, 0.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f);
	~EditCamera();

	inline Vector3f get_direction() const override { return (m_focal_point - m_position).normalize(); }
	inline Vector3f get_focal_point() const { return m_focal_point; }
	inline void set_focal_point(Vector3f const& focal_point) { m_focal_point = focal_point; }

	void update() override;

protected:
	void rotate() override;
	void translate() override;
private:
	void update_view();

	float m_distance; // The distance between the focal point and the camera position.
	Vector3f m_focal_point;
};
