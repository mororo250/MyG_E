#include "Light.h"
#include "imgui.h"

Light::Light(Vector<float, 3> position, Vector<float, 3> color)
	: mColor(color)
	, mMesh(std::make_shared<Mesh>())
	, mModel(mMesh)
	, m_ambient_strength(0.1f)
	, m_diffuse_strength(1.0f)
	, m_specular_strength(1.0f)
{
	mModel.SetPosition(position);
	mModel.SetScale({0.01f, 0.01f, 0.01});
}

Light::~Light()
{
}

void Light::ImGuiRenderer()
{
	ImGui::Begin("Light");
	ImGui::ColorEdit3("Light Color", &mColor[0]);
	ImGui::SliderFloat("Ambient stength:", &m_ambient_strength, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse stength:", &m_diffuse_strength, 0.0f, 5.0f);
	ImGui::SliderFloat("Specular stength:", &m_specular_strength, 0.0f, 5.0f);
	mModel.ImGuiRenderer();
	ImGui::End();
}
