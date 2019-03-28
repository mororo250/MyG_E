#pragma once

class Scene
{
public:
	virtual void ImGuiRenderer() = 0;
	virtual void Update() = 0;
};