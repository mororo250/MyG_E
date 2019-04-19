#pragma once

#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera(Vector<float, 3> position, Vector<float, 3> front_camera);
	~FPSCamera();

	void Update() override;

protected:
	void Rotate() override;
	void Translate() override;
};