#include "ProjectController.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Foundation/ImGuiLayer.h"
#include "Foundation/UI/FileBrowser.h"

ProjectController::ProjectController()
	: m_camera(nullptr)
	, m_skybox_shader(new Shader("SkyBox.glsl"))
	, m_light_shader(new Shader("Light.glsl"))
	, m_shader(new Shader("Phong.glsl"))
	, m_renderer(new Renderer3D())
	, m_fov(60.0f)
	, m_persp_matrix(CreatePerspectiveMatrix(60.0f, Game::Get().get_window_aspect_ratio(), 0.01f))
	, m_skybox(nullptr)
{
}

ProjectController::~ProjectController()
{
	// Delete Objects
	for (auto* aux : m_object_buffer)
		delete aux;

	// Delete Lights
	for (auto* aux : m_light_buffer)
		delete aux;
	
	// Delete Camera
	delete m_camera;
}

void ProjectController::update()
{
	if (m_camera != nullptr)
		m_camera->update();
	else
	{
		std::cout << "There isn't any camera" << std::endl;
		return; // Doen't continue if m_camera = nullptr 
	}

	m_renderer->clear();

	m_shader->bind();

	for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		m_light_buffer[i]->set_uniform(m_shader.get());

	// General uniforms.
	m_shader->set_uniform1i(m_shader->get_uniform_location("u_NumPointLight"), PointLight::get_count());
	m_shader->set_uniform1i(m_shader->get_uniform_location("u_NumSpotLight"), SpotLight::get_count());
	m_shader->set_uniform1i(m_shader->get_uniform_location("u_NumDirectionalLight"), DirectionalLight::get_count());
	m_shader->set_uniform3f(m_shader->get_uniform_location("u_ViewPos"), m_camera->get_position());

	for (auto* aux : m_object_buffer)
	{
		if (aux->is_visible())
		{
			Matrix<float, 4, 4> model_matrix = aux->get_scale_matrix() * aux->get_rotation_matrix() * aux->get_translation(); //Model view projection
			Matrix<float, 4, 4> view_projection = m_camera->get_view() * m_persp_matrix;

			m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_Model"), model_matrix);
			m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_ViewProjection"), view_projection);
			aux->get_material().get_diffuse()->bind(0);
			aux->get_material().get_specular()->bind(1);
			m_shader->set_uniform1i(m_shader->get_uniform_location("u_texture"), 0);
			m_shader->set_uniform1i(m_shader->get_uniform_location("u_specular_map"), 1);
			m_shader->set_uniform1f(m_shader->get_uniform_location("u_shininess"), aux->get_material().get_shininess());
			for (auto& meshes : *aux->get_meshes())
			{
				meshes->GetVertexArray().bind();
				meshes->GetIndexBuffer().bind();
				m_renderer->draw_element(meshes->GetIndexBuffer());
				meshes->GetVertexArray().unbind();
				meshes->GetIndexBuffer().unbind();
			}
			aux->get_material().get_diffuse()->unbind();
		}
	}
	m_shader->unbind();

	// Light
	m_light_shader->bind();
	for (auto* aux : m_light_buffer)
	{
		if (aux->GetModel()->is_visible())
		{
			Matrix<float, 4, 4> model_matrix = aux->GetModel()->get_scale_matrix() * aux->GetModel()->get_rotation_matrix() * aux->GetModel()->get_translation(); // Model view projection
			Matrix<float, 4, 4> view_projection = m_camera->get_view() * m_persp_matrix;

			m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_Model"), model_matrix);
			m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_ViewProjection"), view_projection);
			m_light_shader->set_uniform3f(m_light_shader->get_uniform_location("u_Color"), aux->get_light_color());

			for (auto& meshes : *aux->GetModel()->get_meshes())
			{
				meshes->GetVertexArray().bind();
				meshes->GetIndexBuffer().bind();
				m_renderer->draw_element(meshes->GetIndexBuffer());
				meshes->GetVertexArray().unbind();
				meshes->GetIndexBuffer().unbind();
			}
		}
	}
	m_light_shader->unbind();
	
	// SkyBox
	if (m_skybox)
	{
		m_skybox->begin();
		m_skybox_shader->bind();

		Matrix<float, 4, 4> view_projection = m_camera->get_view() * m_persp_matrix;;
		m_skybox_shader->set_uniformMatrix4f(m_skybox_shader->get_uniform_location("u_view_projection"), view_projection);
		m_skybox_shader->set_uniformMatrix4f(m_skybox_shader->get_uniform_location("scale"), ScaleMatrix4(10000.0f, 10000.0f, 10000.0f)); // this is temporary.
		m_skybox_shader->set_uniform1i(m_skybox_shader->get_uniform_location("u_skybox"), 0);
		m_skybox->draw();
		m_skybox_shader->unbind();
		m_skybox->end();
	}
}

// It need to improve (remove code repretition)
void ProjectController::imgui_renderer()
{
	m_camera->imgui_renderer();

	static bool* p_open;
	ImGui::SetNextWindowSize(ImVec2(250.0f, 400.0f));
	if (ImGui::Begin("Editor", p_open, ImGuiWindowFlags_MenuBar))
	{

		// Left
		static std::size_t selected = 0;
		ImGui::BeginChild("left pane", ImVec2(100, 0), true);
		
		// Lights
		for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		{
			char label[128];
			sprintf(label, "Light %d", i);
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}

		// Objects
		for (std::size_t i = m_light_buffer.size(); i < (m_light_buffer.size() + m_object_buffer.size()); i++)
		{
			char label[128];
			sprintf(label, m_object_buffer[i - m_light_buffer.size()]->get_name().c_str());
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}

		// add item
		if (ImGui::BeginMenu("add"))
		{
			selected = -1;
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Point Light"))
					create_light(0);
				if (ImGui::MenuItem("Spot Light"))
					create_light(1);
				if (ImGui::MenuItem("Directional Light"))
					create_light(2);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Object"))
			{
				if (ImGui::MenuItem("Plane"))
					create_object("..\\..\\..\\Resources\\basic_meshes\\plane.obj");
				if (ImGui::MenuItem("Cube"))
					create_object("..\\..\\..\\Resources\\basic_meshes\\cube.obj");
				if (ImGui::MenuItem("Cone"))
					create_object("..\\..\\..\\Resources\\basic_meshes\\cone.obj");
				if (ImGui::MenuItem("Sphere"))
					create_object("..\\..\\..\\Resources\\basic_meshes\\sphere.obj");
				if (ImGui::MenuItem("Torus"))
					create_object("..\\..\\..\\Resources\\basic_meshes\\torus.obj");
				if (ImGui::MenuItem("Import Mesh"))
				{
					create_object(open_file_browser(L"Model Files", L"*.obj;*.dae;*.fbx;*.bvh;*.ply;*.stl" ));
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		
		// skybox
		ImGui::Separator();

		if (m_skybox)
			m_skybox->imgui_rederer();
		else
			if (ImGui::Button("Attach skybox"))
			{
				set_skybox(new TextureCubMap(open_folder_browser()));
			}
		ImGui::EndChild();
		ImGui::SameLine();
		
		// Right 
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		
		if (selected < m_light_buffer.size() && selected >= 0) {
			ImGui::Text("Light: %d", selected);
			ImGui::Separator();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Features"))
				{
					m_light_buffer[selected]->ImGuiRenderer();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		else if(selected >= m_light_buffer.size() && m_object_buffer.size() + m_light_buffer.size() >= selected)
		{
			ImGui::Text("Object: %d", selected - m_light_buffer.size());
			ImGui::Separator();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Features"))
				{
					m_object_buffer[selected - m_light_buffer.size()]->ImGuiRenderer();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::EndChild();
		ImGui::EndGroup();
	}
	ImGui::End();
}

void ProjectController::pop_light(Light* light)
{
	auto aux = std::find(m_light_buffer.begin(), m_light_buffer.end(), light);
	if (aux != m_light_buffer.end())
		m_light_buffer.erase(aux);
}

void ProjectController::pop_object(Model3D* object)
{
	auto aux = std::find(m_object_buffer.begin(), m_object_buffer.end(), object);
	if (aux != m_object_buffer.end())
		m_object_buffer.erase(aux);
}

void ProjectController::set_perspective_matrix()
{
	m_persp_matrix = CreatePerspectiveMatrix(m_fov, Game::Get().get_window_aspect_ratio(), 0.01f);
}

void ProjectController::set_fov(float const fov)
{
	m_fov = fov;
	set_perspective_matrix();
}

void ProjectController::create_object(std::string const& file_path)
{
	push_object(new Model3D(Model3D::load_model(file_path)));
}

void ProjectController::create_light(unsigned int type)
{
	switch (type)
	{
	case 0:
		push_light(new PointLight({ 0.0f, 0.0f, 0.0f }));
		break;
	case 1:
		push_light(new SpotLight({ 0.0f, 0.0f, 0.0f }));
		break;
	case 2:
		push_light(new DirectionalLight({ 0.0f, 0.0f, 0.0f }));
		break;
	}
}
