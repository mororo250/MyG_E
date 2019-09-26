#pragma once

#include "Foundation\Uncopyable.h"
#include "Foundation\LayerCollection.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ProjectController;
class ImGuiLayer;

//singleton class
class Game : public Uncopyable 
{
private:
	Game();

public:
	bool Initialize();
	void Loop();
	void Shutdown();

	inline GLFWwindow* GetWindow() const { return mWindow; }
	inline float GetDelta() const { return mDelta; }
	inline int GetHeight() const { return mWinHeight; }
	inline int GetWidth() const { return mWinWidth; }

	void open_project(std::string const& path);
	void save_project(std::string const& path);
	void set_project(ProjectController* project_controller);

	inline static Game& Get() 
	{ 
		if (s_Instance == nullptr)
			s_Instance = new Game();
		return *s_Instance; 
	}

private:
	float mDelta; //time between this frame and the last one
	int mWinHeight;
	int mWinWidth;
	GLFWwindow* mWindow;
	static Game *s_Instance;

	LayerCollection m_layer_collection;
	ImGuiLayer* m_imgui_layer;
	ProjectController* m_project_controller;
};