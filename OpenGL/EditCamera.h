#pragma once

#include "Camera.h"

class EditCamera : public Camera
{
public:
	EditCamera(Vector<float, 3>position, Vector<float, 3> mFrontCamera = {0.0f, 0.0f, 0.0f});
	~EditCamera();

	void Update() override;

protected:
	void Rotate() override;
	void Translate() override;

private:
	Vector<float, 3> mDirection; //vector between position and front camera 
};

