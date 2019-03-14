#include "Menu.h"
#include "imgui.h"

Menu::Menu(Scene*& CurrentScene)
:mCurrentScene(CurrentScene)
{
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
