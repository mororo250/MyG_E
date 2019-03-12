#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Game
{
public:

	Game();
	bool Initialize();
	void Loop();
	void Shutdown();

private:
	float mWinHigh = 768.0;
	float mWinLengh = 1024.0;
	GLFWwindow *mWindow;

};