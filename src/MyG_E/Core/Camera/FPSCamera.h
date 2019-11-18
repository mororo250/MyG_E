#pragma once

#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera(Vector3f const& position, float yaw = 0.0f, float pitch = 0.0f);
	FPSCamera(Camera const* other);
	~FPSCamera();

	inline Vector3f get_direction() const override { return m_direction; }
	inline void set_direction(Vector3f const& direction) { m_direction = direction; }

	void update() override;

protected:
	void rotate() override;
	void translate() override;

private:
	Vector3f m_up;
	Vector3f m_direction;
};