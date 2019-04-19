#include "Light.h"
#include "imgui.h"

Light::Light(Vector<float, 3> position, Vector<float, 3> color)
	:mPosition(position), mColor(color)
{
}

Light::~Light()
{
}

void Light::ImGuiRenderer()
{
	ImGui::ColorEdit3("Light Color", &mColor[0]);
	ImGui::SliderFloat3("Light Position", &mPosition[0], -1000.0f, 1000.0f);
}
