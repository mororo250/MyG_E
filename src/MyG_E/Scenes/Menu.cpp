#include "Menu.h"
#include "imgui.h"
#include "Core/Renderer2D.h"

Menu::Menu(Scene*& CurrentScene)
:mCurrentScene(CurrentScene)
{
	m_renderer = std::make_unique<Renderer2D>();
}

Menu::~Menu()
{
}



void Menu::ImGuiRenderer()
{
	for (auto& scene : mScenes)
		if (ImGui::Button(scene.first.c_str()))
			mCurrentScene = scene.second();
}

void Menu::Update()
{
	m_renderer->Clear();
}
