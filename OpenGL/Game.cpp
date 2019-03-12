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
#include "imgui_impl_glfw.h"

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

	float TranX = 512.0f;
	float TranY = 384.0f;
	float ScaleX = 1.0f;
	float ScaleY = 1.0f;
	float Rot = 0.0f; // 

	Matrix<float, 3, 3> Ortho = CreateOrthoMatrix(0, mWinLengh, mWinHigh, 0);
	Matrix<float, 3, 3> TranMat = CreateTranslationMatrix3(TranX, TranY);
	Matrix<float, 3, 3> ScaleMat = CreateScaleMatrix3(ScaleX, ScaleY);
	Matrix<float, 3, 3> RotMat = CreateRotationMatrix3(Rot);
	Matrix<float, 3, 3> WorldTransform = ScaleMat * RotMat * TranMat;
	Matrix<float, 3, 3> MVP = WorldTransform * Ortho;

	int mvp = shader.GetUniformLocation("u_MVP");
	shader.SetUniformMatrix3f(mvp, MVP);

	Texture texture("Resources/PS4.PNG");
	texture.bind(0);
	shader.SetUniform1i(shader.GetUniformLocation("u_texture"), 0);

	va.unbind();
	vb.unbind();
	ib.unbind();
	shader.unbind();

	Renderer renderer;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	
	// Setup Style
	ImGui::StyleColorsDark();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(mWindow))
	{ 
		renderer.Clear();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	
		ImGui::Begin("OpenGL"); 
		ImGui::Text("test");
		ImGui::SliderFloat("Translation X", &TranX, 0.0f, mWinLengh);
		ImGui::SliderFloat("Translation y", &TranY, 0.0f, mWinHigh);
		ImGui::SliderFloat("Scale X", &ScaleX, 0.0f, 100.0f);
		ImGui::SliderFloat("Scale y", &ScaleY, 0.0f, 100.0f);
		ImGui::SliderFloat("Rotation X", &Rot, -6.28f, 6.28f);
		ImGui::End();

			
		WorldTransform = CreateScaleMatrix3(ScaleX, ScaleY) * CreateRotationMatrix3(Rot) * CreateTranslationMatrix3(TranX, TranY);
		MVP = WorldTransform * Ortho;
		
		shader.bind();
		shader.SetUniformMatrix3f(mvp , MVP);

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
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwTerminate();
}
