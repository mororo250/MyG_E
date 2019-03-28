#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//singleton class
class Game 
{
public:

	Game();
	bool Initialize();
	void Loop();
	void Shutdown();

	inline GLFWwindow* GetWindow() const { return mWindow; }
	inline float GetDelta() const { return mDelta; }
	inline static Game& Get() 
	{ 
		if (s_Instance == nullptr)
			s_Instance = new Game();
		return *s_Instance; 
	}

private:
	float mDelta; //time between this frame and the last one
	int mWinHigh;
	int mWinLengh;
	GLFWwindow* mWindow;
	static Game *s_Instance;
};