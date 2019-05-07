// Header file.
#include "EditCamera.h"

// Interns 
#include "Foundation\Math\Quaternion.h"

// Third parties.
#include "imgui.h"


EditCamera::EditCamera(Vector<float, 3> position, Vector<float, 3> front_camera)
{
	mPosition = position;
	mFrontCamera = front_camera;
	mDirection = position - front_camera;
	mView = LookAt(mPosition, mFrontCamera, { 0.0f, 1.0f, 0.0f });
}

EditCamera::~EditCamera()
{
}

void EditCamera::Update()
{
	Translate();
	Rotate();

	mView = LookAt(mPosition, mFrontCamera, { 0.0f, 1.0f, 0.0f });
}

void EditCamera::Rotate()
{
	std::pair<float, float> mouse_current_pos(Input::Get().GetMousePosition());//get current mouse position
	
	if (Input::Get().IsMouseButtonPressed(MOUSE_MBUTTON))
	{
		RotationMatrix3 pitch = RotationMatrix3((mouse_current_pos.second - mMousePos.second) * mSensitivity, AxisUsage::AXIS_X); // x axis rotation
		RotationMatrix3 yaw = RotationMatrix3((mouse_current_pos.first - mMousePos.first) * mSensitivity, AxisUsage::AXIS_Y); //Y axis rotation

		mDirection = mDirection * yaw * pitch;
		mPosition = mFrontCamera + mDirection;
	}

	mMousePos = mouse_current_pos;
}

void EditCamera::Translate()
{
	float moved_distance_length = Input::Get().GetScrollOffset() * mSpeed/10 * mDirection.Length();
	Vector<float, 3> moved_distance = mDirection.GetNormalized() * moved_distance_length;
	
	if (Input::Get().GetScrollOffset() && moved_distance.Length() >= mDirection.Length()) 
		mDirection = mDirection.GetNormalized() * 0.2f;
	else
		mDirection -= moved_distance;
	mPosition = mFrontCamera + mDirection;
	
	Input::Get().SetScrollOffset(0.0);
}
 