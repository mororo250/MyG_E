#include "FPSCamera.h"

FPSCamera::FPSCamera(Vector<float, 3> position, Vector<float, 3> front_camera)
{
	mPosition = position;
	mFrontCamera = front_camera;
	mView = LookAt(mPosition, mFrontCamera, { 0.0f, 1.0f, 0.0f });
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Update()
{
	Translate();
	Rotate();

	mView = LookAt(mPosition, (mPosition + mFrontCamera), { 0.0f, 1.0f, 0.0f });
}

void FPSCamera::Rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition()); //get current mouse position

	RotationMatrix3 pitch = RotationMatrix3((mouse_current_pos.second - mMousePos.second) * mSensitivity, AxisUsage::AXIS_X); // x axis rotation
	RotationMatrix3 yaw = RotationMatrix3((mouse_current_pos.first - mMousePos.first) * mSensitivity, AxisUsage::AXIS_Y); //Y axis rotation
	
	mFrontCamera = mFrontCamera * yaw * pitch;
	mMousePos = mouse_current_pos;
}

void FPSCamera::Translate()
{
	if (Input::Get().IsKeyPressed(KEY_W))
		mPosition += mSpeed * mFrontCamera;
	if (Input::Get().IsKeyPressed(KEY_S))
		mPosition -= mSpeed * mFrontCamera;
	if (Input::Get().IsKeyPressed(KEY_A))
		mPosition -= mSpeed * Cross(mFrontCamera, { 0.0f, 1.0f, 0.0f });
	if (Input::Get().IsKeyPressed(KEY_D))
		mPosition += mSpeed * Cross(mFrontCamera, { 0.0f, 1.0f, 0.0f });

}
