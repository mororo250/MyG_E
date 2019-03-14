#pragma once
#include "Scene.h"
#include <vector>
#include <functional>

class Menu : public Scene
{
public:
	Menu(Scene*& CurrentScene);
	~Menu();

	void ImGuiRenderer() override;
	template<class T>
	void RegisterScne(std::string name)
	{
		mScenes.push_back(std::make_pair(name, []() { return new T(); }));
	}

private:
	Scene* mCurrentScene;
	std::vector<std::pair<std::string, std::function<Scene*()>>> mScenes;
};