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

	void resize_window(int width, int height);

	inline GLFWwindow* GetWindow() const { return m_window; }
	inline float GetDelta() const { return m_delta; }
	inline int GetHeight() const { return m_window_height; }
	inline int GetWidth() const { return m_window_width; }

	void open_project(std::string const& path);
	void save_project(std::string const& path) const;
	void set_project(ProjectController* project_controller);

	inline static Game& Get() 
	{ 
		if (s_Instance == nullptr)
			s_Instance = new Game();
		return *s_Instance; 
	}

private:
	float m_delta; //time between this frame and the last one
	int m_window_height;
	int m_window_width;
	GLFWwindow* m_window;
	static Game *s_Instance;

	LayerCollection m_layer_collection;
	ImGuiLayer* m_imgui_layer;
	ProjectController* m_project_controller;
};