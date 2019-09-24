#include <iostream>
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
#include "Foundation/Project/ProjectFileWriter.h"

Game* Game::s_Instance = nullptr;

Game::Game()
	:mWinHeight(768),
	mWinWidth(1024),
	mDelta(1.0f),
	m_imgui_layer(nullptr),
	m_project_controller(nullptr)
{
	try
	{
		if (s_Instance != nullptr)
			throw "EROOR!! Game already exist";
		s_Instance = this;
	}
	catch (const char* msg) 
	{
		std::cerr << msg << std::endl;
	}
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

	// layers
	m_imgui_layer = new ImGuiLayer();

	m_layer_collection.push_layer(m_imgui_layer);

	/* Loop until the user closes the window */
	std::chrono::duration<float> frametime;
	while (!glfwWindowShouldClose(mWindow))
	{
		GLcall(glClear(GL_COLOR_BUFFER_BIT));
		m_imgui_layer->begin();

		for (auto& aux : m_layer_collection)
			aux->update();

		for (unsigned int i = 0; i < m_layer_collection.count(); i++)
			m_layer_collection[i]->imgui_renderer();

		m_imgui_layer->end();

		// Swap front and back buffers 
		glfwSwapBuffers(mWindow);

		// Poll for and process events 
		glfwPollEvents();
	}
}

void Game::Shutdown()
{
	ProjectFileWriter test;
	test.write_file(m_project_controller, "C://Users//joao_//OneDrive//Desktop\git//MyG_E//examples//test.json");
	glfwTerminate();
}

void Game::open_project(std::string path)
{
	ProjectFileReader file_reader;
	if (file_reader.read_file(path))
		std::cout << "Project in" << path << " was successfully loaded" << std::endl;
	else
		std::cout << "It 'wasn't possible to opne:" << path << std::endl;

}

void Game::set_project(ProjectController* project_controller)
{
	if (m_project_controller != nullptr)
	{
		m_layer_collection.pop_layer(m_project_controller);
		delete m_project_controller;
	}
	m_project_controller = project_controller;
	m_layer_collection.push_layer(m_project_controller);
}
