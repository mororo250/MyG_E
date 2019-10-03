#pragma once

#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera(Vector<float, 3> position, Vector<float, 3> direction);
	~FPSCamera();

	void update() override;

protected:
	void rotate() override;
	void translate() override;
};