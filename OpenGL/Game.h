#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Game
{
public:

	Game();
	bool Initialize();
	void Loop();
	void Shutdown();

private:
	int mWinHigh = 768;
	int mWinLengh = 1024;
	GLFWwindow *mWindow;

};