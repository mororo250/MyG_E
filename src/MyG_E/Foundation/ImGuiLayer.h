#pragma once

#include "Foundation/Uncopyable.h"
#include "Foundation/Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void update() override;
	void event() override;
private:
	float m_time;
};