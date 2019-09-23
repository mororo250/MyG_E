#include "Input.h"

Input* Input::s_Instance = nullptr;

Input::Input()
	: mCurrentMode(CURSOR_NORMAL)
	, mWindow(Game::Get().GetWindow())
	, mScrollOffset(0.0)
{
	try
	{
		if (s_Instance != nullptr)
			throw "Game already exist";
		else
			s_Instance = this;
	}
	catch (const char* msg)
	{
		std::cerr << msg << std::endl;
	}

	glfwSetScrollCallback(mWindow, 
		[](GLFWwindow * window, double xoffset, double yoffset)
	{
		Input::Get().SetScrollOffset(yoffset);
	});
}

Input::~Input()
{
}

bool Input::IsKeyPressed(KeyCode keycode)
{
	auto state = glfwGetKey(mWindow, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseButtonCode mousebutton)
{
	auto state = glfwGetMouseButton(mWindow, mousebutton);
	return state == GLFW_PRESS;
}



std::pair<float, float> Input::GetMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(mWindow, &xpos, &ypos);
	return {static_cast<float>(xpos), static_cast<float>(ypos)};
}

void Input::SetCursorMode(CursorMode mode)
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, mode);
	mCurrentMode = mode;
}