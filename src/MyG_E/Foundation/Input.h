#pragma once

#include "Game.h"
#include "Foundation\Uncopyable.h"


//keycodes using glfw numbers
enum KeyCode 
{
	KEY_A = 65,
	KEY_B = 66,
	KEY_C = 67,
	KEY_D = 68,
	KEY_E = 69,
	KEY_F = 70,
	KEY_G = 71,
	KEY_H = 72,
	KEY_I = 73,
	KEY_J = 74,
	KEY_K = 75,
	KEY_L = 76,
	KEY_M = 77,
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
	CURSOR_NORMAL = GLFW_CURSOR_NORMAL, // Makes the cursor visible and behaving normally.
	CURSOR_HIDDEN = GLFW_CURSOR_HIDDEN, // Makes the cursor invisible when it is over the client area of the window but does not restrict the cursor from leaving.
	CURSOR_CAMERA3D = GLFW_CURSOR_DISABLED // Hides and grabs the cursor, providing virtual and unlimited cursor movement
};

class Input : public Uncopyable
{
private:
	Input();
public:
	~Input();
	
	bool IsKeyPressed(KeyCode keycode);
	bool IsMouseButtonPressed(MouseButtonCode mousebutton);
	std::pair<float, float> GetMousePosition() const;

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

