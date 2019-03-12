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
	GLFWwindow *mWindow;
};