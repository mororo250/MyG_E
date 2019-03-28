#include "Input.h"

Input* Input::s_Instance = nullptr;

Input::Input()
	:mCurrentMode(CURSOR_NORMAL)
{
	if (s_Instance != nullptr)
	{
		std::cout << "Instance already existed" << std::endl;
		__debugbreak();
	}
	else
		s_Instance = this;
}

Input::~Input()
{
}

bool Input::IsKeyPressed(KeyCode keycode)
{
	auto window = Game::Get().GetWindow();
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseButtonCode mousebutton)
{
	auto window = Game::Get().GetWindow();
	auto state = glfwGetMouseButton(window, mousebutton);
	return state == GLFW_PRESS;
}



std::pair<float, float> Input::GetMousePosition()
{
	auto window = Game::Get().GetWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return {static_cast<float>(xpos), static_cast<float>(ypos)};
}

void Input::SetCursorMode(CursorMode mode)
{
	auto window = Game::Get().GetWindow();
	glfwSetInputMode(window, GLFW_CURSOR, mode);
	mCurrentMode = mode;
}