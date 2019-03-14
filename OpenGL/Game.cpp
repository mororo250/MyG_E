#include <iostream>
#include <string>
#include <memory>

#include "Game.h"
#include "Renderer.h"
#include "Texture.h"

//imgui
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "Texture2D.h"
#include "Menu.h"


Game::Game()
:mWindow(nullptr)
{
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

	if (glewInit() != GLEW_OK) {
		std::cout << "error!";
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
	/* Loop until the user closes the window */

	while (!glfwWindowShouldClose(mWindow))
	{ 
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (CurrentScene)
		{
			ImGui::Begin("OpenGL");
			if (CurrentScene != menu && ImGui::Button("<-"))
			{
				//delete CurrentScene; //error 1281 when I delete Current Scene - Don't seam to make difference
				CurrentScene = menu; 
			}

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
	}
}

void Game::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwTerminate();
}
