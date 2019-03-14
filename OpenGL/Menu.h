#pragma once
#include "Scene.h"
#include <vector>
#include <functional>

class Menu : public Scene
{
	Menu();
	~Menu();

	void ImGuiRenderer() override;

private:
	Scene* mCurrentScene;
	std::vector<std::pair<std::string, std::function<Scene*()>>> mScenes;
};