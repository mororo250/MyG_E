#include "ImGuiLayer.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "Foundation/Game.h"
#include "Foundation/UI/FileBrowser.h"
#include "Foundation/Project/ProjectFileWriter.h"

ImGuiLayer::ImGuiLayer()
	:m_time(0.0)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	m_iniFilename = std::filesystem::absolute("..//..//..//imgui.ini").string();
	io.IniFilename = m_iniFilename.c_str();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


	ImGui_ImplGlfw_InitForOpenGL(Game::Get().GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::update()
{
	ImGuiIO& io = ImGui::GetIO();
	float time = static_cast<float>(glfwGetTime());
	io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
	m_time = time;
}

void ImGuiLayer::event()
{
}

void ImGuiLayer::imgui_renderer()
{
	// Use this to learn how to properly use imgui
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);
	docking();
	overlay_info();
	show_menu();
}

void ImGuiLayer::begin()
{
	ImGuiIO& io = ImGui::GetIO();
	Game& app = Game::Get();
	std::pair<int, int> display_size(app.get_window_size());
	io.DisplaySize = ImVec2(static_cast<float>(display_size.first), static_cast<float>(display_size.second));

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::end()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiLayer::docking()
{
	static bool show = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	}

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &show, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		// Set dock layout
		/*
		if (!ImGui::DockBuilderGetNode(dockspace_id))
		{
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, 0); // Add empty node
			ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
		
			ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
			ImGuiID dock_id_prop = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
			ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_id_prop, ImGuiDir_Down, 0.20f, nullptr, &dock_id_prop);
		
			ImGui::DockBuilderDockWindow("Camera_infomation", dock_id_bottom);
			ImGui::DockBuilderDockWindow("Editor", dock_id_prop);
			ImGui::DockBuilderFinish(dockspace_id);
		}
		*/
		ImGui::DockSpace(dockspace_id, ImGui::GetMainViewport()->Size, dockspace_flags);
	}

	ImGui::End();
}

void ImGuiLayer::overlay_info()
{
	static bool show = true;
	ImGuiIO& io = ImGui::GetIO();
	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 window_pos = ImVec2((viewport->Pos.x + viewport->Size.x), (viewport->Pos.y + viewport->Size.y));
	ImVec2 window_pos_pivot = ImVec2();
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(1.01f, 1.01f));
	ImGui::SetNextWindowViewport(viewport->ID);
	
	ImGui::SetNextWindowBgAlpha(0.20f); // Transparent background
	if (ImGui::Begin("info", &show, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | 
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
	}
	ImGui::End();
}

void ImGuiLayer::show_menu()
{
	// Main menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			show_menu_file();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			show_menu_edit();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ImGuiLayer::show_menu_file()
{
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) 
	{
		std::string project_file = open_file_browser(L".Project files", L"*.json");
		if (!project_file.empty())
			Game::Get().open_project(project_file);
	}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As.."))
	{
		std::string project_file = save_file_browser(L".json", L"*.json");
		if (!project_file.empty())
			Game::Get().save_project(project_file);
	}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Quit", "Alt+F4")) 
		glfwSetWindowShouldClose(Game::Get().GetWindow(), GLFW_TRUE);
}

void ImGuiLayer::show_menu_edit()
{
	if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
	if (ImGui::MenuItem("Redo", "CTRL+Y")) {}
}

