#include "Menu.h"
#include "imgui.h"
#include "Core/Renderer.h"

Menu::Menu(Scene*& CurrentScene)
:mCurrentScene(CurrentScene)
{
	mRenderer = std::make_unique<Renderer>();
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
	mRenderer->Clear();
}
