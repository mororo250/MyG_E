#include "Camera.h"


Camera::Camera(Vector<float, 3> Position, Vector<float, 3> FrontCamera)
	:mPosition(Position), 
	mFrontCamera(FrontCamera)
{
	mView = LookAt(mPosition, mFrontCamera, { 0.0f, 1.0f, 0.0f });
}

Camera::~Camera()
{
}

void Camera::Update(Vector<float, 3> position, Vector<float, 3> frontcamera)
{
	mPosition = position;
	mFrontCamera = frontcamera;
	mView = LookAt(mPosition, mPosition + mFrontCamera, { 0.0f, 1.0f, 0.0f });
}