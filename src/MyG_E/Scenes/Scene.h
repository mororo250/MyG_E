#pragma once

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void ImGuiRenderer() = 0;
	virtual void Update() = 0;
};