#include <iostream>
#include <string>
#include <memory>
#include <chrono> //time

#include "Game.h"

//imgui
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

// Scenes
#include "Scenes/Texture2D.h"
#include "Scenes/Menu.h"
#include "Scenes/BatchRenderScene.h"
#include "Scenes/Render3DScene.h"

// Layers
#include "Foundation/Layer.h"
#include "Foundation/ImGuiLayer.h"
#include "Core/ProjectController.h"

// Others
#include "Foundation/Project/ProjectFileReader.h"

Game* Game::s_Instance = nullptr;

Game::Game()
	:mWinHeight(768),
	mWinWidth(1024),
	mDelta(1.0f)
{
	if (s_Instance != nullptr)
	{
		std::cout << "Game already existed" << std::endl;
		__debugbreak();
	}
	else
		s_Instance = this;
}

bool Game::Initialize()
{

	/* Initialize the library */
	if (!glfwInit())
		return 0;

	/* use a multisample buffer with N samples instead of a normal color buffer */
	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	mWindow = glfwCreateWindow(mWinWidth, mWinHeight, "Hello World", NULL, NULL);
	if (!mWindow)
	{
		glfwTerminate();
		return 0;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(mWindow);
	//sicronize with vsync
	glfwSwapInterval(1);

	if (!gladLoadGL()) {
		printf("Something went wrong!\n");
		return 0;
	}
	
	return 1;

}

void Game::Loop()
{
	// Setup some opengl cofiguration
	GLcall(glEnable(GL_MULTISAMPLE));

	ProjectFileReader* test = new ProjectFileReader();
	if (test->read_file(std::string("..\\..\\..\\examples\\3d_scene.json")))
		std::cout << "SUCCESS" << std::endl;
	else
		std::cout << "ERROR" << std::endl;

	// layers
	m_imgui_layer = new ImGuiLayer();
	m_project_controller = new ProjectController();

	m_layer_collection.push_layer(m_imgui_layer);
	m_layer_collection.push_layer(m_project_controller);

	/* Loop until the user closes the window */
	std::chrono::duration<float> frametime;
	while (!glfwWindowShouldClose(mWindow))
	{
		GLcall(glClear(GL_COLOR_BUFFER_BIT));
		m_imgui_layer->begin();

		for (auto& aux : m_layer_collection)
			aux->update();

		for (auto& aux : m_layer_collection)
			aux->imgui_renderer();

		m_imgui_layer->end();

		// Swap front and back buffers 
		glfwSwapBuffers(mWindow);

		// Poll for and process events 
		glfwPollEvents();
	}
}

void Game::Shutdown()
{
	glfwTerminate();
}
