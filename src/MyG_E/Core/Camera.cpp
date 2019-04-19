#include "Camera.h"
#include "imgui.h"

Camera::Camera(float speed, float sensitivity)
	:mSpeed(speed),
	mSensitivity(sensitivity),
	mMousePos(std::move(Input::Get().GetMousePosition()))
{
}

Camera::~Camera()
{
}

void Camera::ImGuiRenderer()
{
	ImGui::Text("Mouse Position: %f %f", mMousePos.first, mMousePos.second);
	ImGui::SliderFloat("Speed: ", &mSpeed, 0.0f, 10.0f);
	ImGui::SliderFloat("Sensitivity: ", &mSensitivity, 0.010, 0.000);
}
