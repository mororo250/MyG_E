#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void ImGuiRenderer();
	virtual void Update();
};