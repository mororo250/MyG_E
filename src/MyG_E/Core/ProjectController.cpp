#include "ProjectController.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Foundation/ImGuiLayer.h"
#include "Foundation/UI/FileBrowser.h"

ProjectController::ProjectController()
	: m_camera(nullptr)
	, m_skybox_shader(new Shader("SkyBox.vert", "SkyBox.frag"))
	, m_normal_shader(new Shader("Normal.vert", "Normal.frag", "Normal.geom"))
	, m_light_shader(new Shader("Light.vert", "Light.frag"))
	, m_shader(new Shader("Phong.vert", "Blinn-Phong.frag"))
	, m_renderer(new Renderer3D())
	, m_fov(60.0f)
	, m_persp_matrix(CreatePerspectiveMatrix(60.0f, Game::Get().get_window_aspect_ratio(), 0.01f))
	, m_skybox(nullptr)
	, m_flags(flags_draw_skybox)
	, m_current_shader(BLINN_PHONG)
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
}

void ProjectController::update()
{
	if (!m_renderer->is_active())
		m_renderer->set_active(); // Ideally set it as active just when changing current project.

	if (m_camera != nullptr)
		m_camera->update();
	else
	{
		std::cout << "There isn't any camera" << std::endl;
		return; // Doen't continue if m_camera = nullptr 
	}

	m_renderer->clear();

	// Draw Objects.
	{
		m_shader->bind();

		// General uniforms.
		m_shader->set_uniform1i(m_shader->get_uniform_location("u_NumPointLight"), PointLight::get_count());
		m_shader->set_uniform1i(m_shader->get_uniform_location("u_NumSpotLight"), SpotLight::get_count());
		m_shader->set_uniform1i(m_shader->get_uniform_location("u_NumDirectionalLight"), DirectionalLight::get_count());
		m_shader->set_uniform3f(m_shader->get_uniform_location("u_ViewPos"), m_camera->get_position());
		m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_View"), m_camera->get_view());
		m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_Projection"), m_persp_matrix);

		// Light uniforms
		for (unsigned int i = 0; i < m_light_buffer.size(); i++)
			m_light_buffer[i]->set_uniform(m_shader.get());

		for (auto* aux : m_object_buffer)
		{
			if (aux->is_visible())
			{
				Matrix<float, 4, 4> model_matrix = aux->get_scale_matrix() * aux->get_rotation_matrix() * aux->get_translation(); // Model view projection.
				m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_Model"), model_matrix);
				m_shader->set_uniform2f(m_shader->get_uniform_location("u_scale_uv"), aux->get_material()->get_diffuse()->get_scale_uv());

				aux->get_material()->get_diffuse()->bind(0);
				aux->get_material()->get_specular()->bind(1);
				aux->get_material()->get_normal_map()->bind(2);
				m_shader->set_uniform1i(m_shader->get_uniform_location("u_texture"), 0);
				m_shader->set_uniform1i(m_shader->get_uniform_location("u_specular_map"), 1);
				m_shader->set_uniform1i(m_shader->get_uniform_location("u_nomal_map"), 2);
				m_shader->set_uniform1i(m_shader->get_uniform_location("u_is_using_normal_map"), aux->get_material()->is_using_normal_map());
				m_shader->set_uniform1f(m_shader->get_uniform_location("u_shininess"), aux->get_material()->get_shininess());
				for (auto& meshes : *aux->get_meshes())
				{
					meshes->GetVertexArray().bind();
					meshes->GetIndexBuffer().bind();
					m_renderer->draw_element(meshes->GetIndexBuffer());
					meshes->GetVertexArray().unbind();
					meshes->GetIndexBuffer().unbind();
				}
				aux->get_material()->get_diffuse()->unbind();
				aux->get_material()->get_specular()->unbind();
			}
		}
		m_shader->unbind();
	}

	// Draw Normals
	if (m_flags & flags_draw_normals)
	{
		m_normal_shader->bind();
		m_normal_shader->set_uniformMatrix4f(m_normal_shader->get_uniform_location("u_View"), m_camera->get_view());
		m_normal_shader->set_uniformMatrix4f(m_normal_shader->get_uniform_location("u_Projection"), m_persp_matrix);
		for (auto* aux : m_object_buffer)
		{
			if (aux->is_visible())
			{
				Matrix<float, 4, 4> model_matrix = aux->get_scale_matrix() * aux->get_rotation_matrix() * aux->get_translation(); //Model view projection
				m_shader->set_uniformMatrix4f(m_normal_shader->get_uniform_location("u_Model"), model_matrix);

				for (auto& meshes : *aux->get_meshes())
				{
					meshes->GetVertexArray().bind();
					meshes->GetIndexBuffer().bind();
					m_renderer->draw_element(meshes->GetIndexBuffer());
					meshes->GetVertexArray().unbind();
					meshes->GetIndexBuffer().unbind();
				}
			}
		}
		m_normal_shader->unbind();
	}

	// Light.
	if (m_flags & flags_draw_lights)
	{
		m_light_shader->bind();
		m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_View"), m_camera->get_view());
		m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_Projection"), m_persp_matrix);
		for (auto* aux : m_light_buffer)
		{
			if (aux->GetModel()->is_visible())
			{
				Matrix<float, 4, 4> model_matrix = aux->GetModel()->get_scale_matrix() * aux->GetModel()->get_rotation_matrix() * aux->GetModel()->get_translation(); // Model view projection
				m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_Model"), model_matrix);
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
	}
	
	// SkyBox.
	if (m_flags & flags_draw_skybox)
	{
		if (m_skybox)
		{
			m_skybox->begin();
			m_skybox_shader->bind();

			m_skybox_shader->set_uniformMatrix4f(m_skybox_shader->get_uniform_location("u_Projection"), m_persp_matrix);
			m_skybox_shader->set_uniformMatrix4f(m_skybox_shader->get_uniform_location("u_View"), m_camera->get_view());
			m_skybox_shader->set_uniformMatrix4f(m_skybox_shader->get_uniform_location("scale"), ScaleMatrix4(10000.0f, 10000.0f, 10000.0f)); // this is temporary.
			m_skybox_shader->set_uniform1i(m_skybox_shader->get_uniform_location("u_skybox"), 0);
			m_skybox->draw();
			m_skybox_shader->unbind();
			m_skybox->end();
		}
	}
}

// It need to improve (remove code repretition)
void ProjectController::imgui_renderer()
{
	m_camera->imgui_renderer();

	static bool* models_p_open;
	static bool* scene_p_open;
	if (ImGui::Begin("Models", models_p_open, ImGuiWindowFlags_MenuBar))
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
	
		ImGui::EndChild();
		ImGui::SameLine();
		
		// Right 
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		
		if (selected < m_light_buffer.size() && selected >= 0) 
		{
			ImGui::Text("Light: %d", selected);
			ImGui::Separator();
			m_light_buffer[selected]->ImGuiRenderer();
		}
		else if(selected >= m_light_buffer.size() && m_object_buffer.size() + m_light_buffer.size() >= selected)
		{
			ImGui::Text("Object: %d", selected - m_light_buffer.size());
			ImGui::Separator();
			m_object_buffer[selected - m_light_buffer.size()]->ImGuiRenderer();
		}
		ImGui::EndChild();

		ImGui::SetCursorPos(ImVec2(0, 500.0f));
	}
	ImGui::End();

	if (ImGui::Begin("Project Controller", scene_p_open, ImGuiWindowFlags_MenuBar))
	{
		// add item
		if (ImGui::BeginMenu("add"))
		{
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
					create_object(open_file_browser(L"Model Files", L"*.obj;*.dae;*.fbx;*.bvh;*.ply;*.stl"));
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		// skybox
		if (m_skybox)
			m_skybox->imgui_rederer();
		else
			if (ImGui::Button("Attach skybox"))
				set_skybox(new TextureCubMap(open_folder_browser()));

		char const* camera[] = { "Edit Camera", "Fps Camera" };
		static int camera_idx = 0;
		ImGui::Combo("Camera", &camera_idx, camera, sizeof(camera) / sizeof(char*));
		if (m_current_camera != current_camera(camera_idx))
			set_current_camera(camera_idx);

		// Shader
		char const* shader[] = { "Blinn Phong", "Phong" };
		static int shader_idx = 0;
		ImGui::Combo("Shader", &shader_idx, shader, sizeof(shader) / sizeof(char*));
		if (m_current_shader != current_shader(shader_idx))
			set_current_shader(shader_idx);

		// Flags
		if (ImGui::CollapsingHeader("Flags", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::CheckboxFlags("Draw Light", reinterpret_cast<unsigned int*>(&m_flags), flags_draw_lights);
			ImGui::CheckboxFlags("Draw skybox", reinterpret_cast<unsigned int*>(&m_flags), flags_draw_skybox);
			ImGui::CheckboxFlags("Draw Normals", reinterpret_cast<unsigned int*>(&m_flags), flags_draw_normals);
		}
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

void ProjectController::set_camera(Camera* camera)
{
	m_camera.reset(camera);
	if (dynamic_cast<EditCamera const*>(m_camera.get()))
		m_current_camera = EDIT_CAMERA;
	if (dynamic_cast<FPSCamera const*>(m_camera.get()))
		m_current_camera = FPS_CAMERA;
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
	push_object(new Model3D(nullptr, file_path));
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

void ProjectController::set_current_shader(unsigned int index)
{
	m_current_shader = current_shader(index);
	switch (m_current_shader)
	{
	case BLINN_PHONG:
		m_shader.reset(new Shader("Phong.vert", "Blinn-Phong.frag"));
		break;
	case PHONG:
		m_shader.reset(new Shader("Phong.vert", "Phong.frag"));
		break;
	}
}

void ProjectController::set_current_camera(unsigned int index)
{
	m_current_camera = current_camera(index);
	Vector3f pos = m_camera->get_position();
	Vector3f dir = m_camera->get_direction();
	switch (m_current_camera)
	{
	case EDIT_CAMERA:
		m_camera.reset(new EditCamera(pos, dir));
		break;
	case FPS_CAMERA:
		m_camera.reset(new FPSCamera(pos, dir));
		break;
	}
}
