#pragma once

#include <iostream>
#include "Game.h"


//keycodes using glfw numbers
enum KeyCode 
{
	KEY_A = 65,
	KEY_D = 68,
	KEY_S = 83,
	KEY_W = 87,
	KEY_LSHIFT = 340,
	KEY_LCRTL = 341
};

enum MouseButtonCode
{
	MOUSE_LBUTTON = 0,
	MOUSE_RBUTTON = 1,
	MOUSE_MBUTTON = 2
};

enum CursorMode
{
	CURSOR_NORMAL = GLFW_CURSOR_NORMAL, //makes the cursor visible and behaving normally.
	CURSOR_HIDDEN = GLFW_CURSOR_HIDDEN, //makes the cursor invisible when it is over the client area of the window but does not restrict the cursor from leaving.
	CURSOR_CAMERA3D = GLFW_CURSOR_DISABLED // hides and grabs the cursor, providing virtual and unlimited cursor movement
};

class Input
{
private:
	Input();
public:
	~Input();
	
	bool IsKeyPressed(KeyCode keycode);
	bool IsMouseButtonPressed(MouseButtonCode mousebutton);
	std::pair<float, float> GetMousePosition();

	void SetCursorMode(CursorMode mode);
	inline CursorMode GetCurrentCursorMode() const { return mCurrentMode; }

	inline void SetScrollOffset(double offset) { mScrollOffset = offset; }
	inline double GetScrollOffset() const { return mScrollOffset; }

	inline static Input& Get() 
	{ 
		if (s_Instance == nullptr)
			s_Instance = new Input();
		return *s_Instance; 
	}
private:
	static Input* s_Instance;
	CursorMode mCurrentMode;
	GLFWwindow* mWindow;
	double mScrollOffset;
};

