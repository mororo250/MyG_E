#include <iostream>
#include <string>
#include <memory>

#include "Gldebug.h"
#include "Game.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "shader.h"
#include "Renderer.h"
#include "Texture.h"

//imgui
#include "imgui.h"
#include "imgui_impl_opengl3.h"

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
	mWindow = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
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
	float positions[] = {
		-50.0f,  -50.0f , 0.0f, 0.0f, //index 0
		50.0f ,  -50.0f , 1.0f, 0.0f, //index 1
		50.0f ,  50.0f, 1.0f, 1.0f, //index 2
		-50.0f,  50.0f, 0.0f, 1.0f  //index 3
	};

	unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
	};

	GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
	VertexArray  va;
	VertexBuffer vb(positions, 4, 4);
	IndexBuffer  ib(indices, 6);

	va.PushLayout(2, GL_FLOAT, GL_FALSE, 0);
	va.PushLayout(2, GL_FLOAT, GL_FALSE, 2);
	va.AddBuffer(vb);

	Shader shader("Shader.shader");
	shader.bind();

	Matrix<float, 3, 3> Ortho = CreateOrthoMatrix(0, 1024, 768, 0);
	Matrix<float, 3, 3> TranMat = CreateTranslationMatrix3(450.0f, 300.0f);
	Matrix<float, 3, 3> ScaleMat = CreateScaleMatrix3(1.0f, 1.0f);
	Matrix<float, 3, 3> RotMat = CreateRotationMatrix3(0);
	Matrix<float, 3, 3> WorldTransform = ScaleMat * RotMat * TranMat;
	Matrix<float, 3, 3> MVP = WorldTransform * Ortho;
	shader.SetUniformMatrix3f(shader.GetUniformLocation("u_MVP"), MVP);

	Texture texture("Resources/PS4.PNG");
	texture.bind(0);
	shader.SetUniform1i(shader.GetUniformLocation("u_texture"), 0);

	va.unbind();
	vb.unbind();
	ib.unbind();
	shader.unbind();

	Renderer renderer;

	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(mWindow))
	{ 
		renderer.Clear();
		
		ImGui_ImplOpenGL3_NewFrame();

		renderer.Draw(va, ib, shader);

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
	ImGui::DestroyContext();
	glfwTerminate();
}
