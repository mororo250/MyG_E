#include <iostream>
#include <string>
#include <memory>
#include <chrono> //time

#include "Game.h"

//imgui
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "Scenes/Texture2D.h"
#include "Scenes/Menu.h"
#include "Scenes/BatchRenderScene.h"
#include "Scenes/Render3DScene.h"

Game* Game::s_Instance = nullptr;

Game::Game()
	:mWinHigh(768),
	mWinLengh(1024),
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

	/* Create a windowed mode window and its OpenGL context */
	mWindow = glfwCreateWindow(mWinLengh, mWinHigh, "Hello World", NULL, NULL);
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
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	
	// Setup Style
	ImGui::StyleColorsDark();

	Scene* CurrentScene = nullptr;
	Menu* menu = new Menu(CurrentScene);
	CurrentScene = menu;
	menu->RegisterScne<Texture2D>("Texture2D Scene");
	menu->RegisterScne<BatchRenderScene>("Batch Rendering Scene");
	menu->RegisterScne<Render3DScene>("render3D Scene");

	/* Loop until the user closes the window */
	std::chrono::duration<float> frametime;
	while (!glfwWindowShouldClose(mWindow))
	{ 
		auto start = std::chrono::high_resolution_clock::now();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (CurrentScene)
		{
			ImGui::Begin("OpenGL");
			if (CurrentScene != menu && ImGui::Button("<-"))
			{
				delete CurrentScene; //error 1281 when I delete Current Scene - Don't seam to make difference
				CurrentScene = menu; 
			}
			ImGui::Text("fps: %f", 1.0f/mDelta);
			CurrentScene->ImGuiRenderer();
			ImGui::End();
 			CurrentScene->Update();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		/* Swap front and back buffers */
		GLcall(glfwSwapBuffers(mWindow));

		/* Poll for and process events */
		GLcall(glfwPollEvents());
		auto end = std::chrono::high_resolution_clock::now();
		frametime = end - start;
		mDelta = frametime.count();
	}
	if (CurrentScene != menu)
		delete CurrentScene;
	delete menu;
}

void Game::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwTerminate();
}
