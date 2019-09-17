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
	void imgui_renderer() override;
	
	void begin();
	void end();

private:
	void docking();
	void show_menu();
	void show_menu_file();
	void show_menu_edit();
	
	float m_time;
};