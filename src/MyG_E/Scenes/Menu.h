#pragma once
#include "Scene.h"
#include <vector>
#include <iostream>
#include <functional>

class Menu : public Scene
{
public:
	Menu(Scene*& CurrentScene);
	~Menu();

	void ImGuiRenderer() override;
	void Update() override;

	template<class T>
	void RegisterScne(std::string name)
	{
		mScenes.push_back(std::make_pair(name, []() { return new T(); }));
	}

private:
	Scene*& mCurrentScene;
	std::vector<std::pair<std::string, std::function<Scene*()>>> mScenes;
	std::unique_ptr<class Renderer> mRenderer;
};