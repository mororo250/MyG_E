#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//imgui
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

// Layers
#include "Foundation/Layer.h"
#include "Foundation/ImGuiLayer.h"
#include "Core/ProjectController.h"

// Others
#include "Foundation/Project/ProjectFileReader.h"
#include "Foundation/Project/ProjectFileWriter.h"
#include "Foundation/Gldebug.h"

Game* Game::s_Instance = nullptr;

Game::Game()
	: m_delta(1.0f)
	, m_imgui_layer(nullptr)
	, m_project_controller(nullptr)
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
	m_window = glfwCreateWindow(1024, 768, "Pixie Game Engine", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		return 0;
	}

	glfwSetWindowSizeCallback(m_window, 
		[](GLFWwindow* window, int width, int height)
	{
		Game::Get().resize_window(width, height);
	}
		);

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);
	//sicronize with vsync
	glfwSwapInterval(0);

	if (!gladLoadGL()) {
		printf("Something went wrong!\n");
		return 0;
	}
	
	return 1;

}

void Game::Loop()
{
	// layers
	m_imgui_layer = new ImGuiLayer();

	m_layer_collection.push_layer(m_imgui_layer);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		m_imgui_layer->begin();

		for (auto& aux : m_layer_collection)
			aux->update();

		for (unsigned int i = 0; i < m_layer_collection.count(); i++)
			m_layer_collection[i]->imgui_renderer();

		m_imgui_layer->end();

		// Swap front and back buffers 
		glfwSwapBuffers(m_window);

		// Poll for and process events 
		glfwPollEvents();
	}
}

void Game::Shutdown()
{
	glfwTerminate();
}

void Game::resize_window(int width, int height)
{
	GLcall(glViewport(0, 0, width, height));
	m_project_controller->set_perspective_matrix();
}

Vector2i Game::get_window_size() const
{
	Vector2i window_size;
	glfwGetWindowSize(m_window, &window_size[0], &window_size[1]);
	return window_size;
}

float Game::get_window_aspect_ratio() const
{
	Vector2i window_size = get_window_size();
	return static_cast<float>(window_size[0]) / static_cast<float>(window_size[1]);
}

void Game::open_project(std::string const& path)
{
	ProjectFileReader file_reader;
	if (file_reader.read_file(path))
		std::cout << "Project in" << path << " was successfully loaded" << std::endl;
	else
		std::cout << "It 'wasn't possible to open:" << path << std::endl;

}

void Game::save_project(std::string const& path) const
{
	ProjectFileWriter file_writer;
	if (file_writer.write_file(m_project_controller, path))
		std::cout << "Project was successfully saved in " << path << std::endl;
	else
		std::cout << "It 'wasn't possible to save project in:" << path << std::endl;
}

void Game::set_project(ProjectController* project_controller)
{
	if (m_project_controller)
	{
		m_layer_collection.pop_layer(m_project_controller);
		delete m_project_controller;
	}
	m_project_controller = project_controller;
	m_layer_collection.push_layer(m_project_controller);
}
