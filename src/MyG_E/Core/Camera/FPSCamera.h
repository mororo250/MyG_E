#pragma once

#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera(Vector3f position, Vector3f direction);
	~FPSCamera();

	void update() override;

protected:
	void rotate() override;
	void translate() override;
};