#pragma once

#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera(Vector<float, 3> position, Vector<float, 3> front_camera);
	~FPSCamera();

	inline Vector<float, 3> GetDirection() const { return m_front_camera; }
	void SetDirection(Vector<float, 3> front_camera) { m_front_camera = front_camera; }

	void Update() override;

protected:
	void Rotate() override;
	void Translate() override;

private:
	Vector<float, 3> m_front_camera;
};