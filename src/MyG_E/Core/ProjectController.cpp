#include "ProjectController.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "glad/glad.h"

#include "Core//Post-Processing/GaussianBlur.h"
#include "Foundation/ImGuiLayer.h"
#include "Foundation/UI/FileBrowser.h"

ProjectController::ProjectController()
	: m_texture(BasicTexture2D::LINEAR, BasicTexture2D::LINEAR, BasicTexture2D::CLAMP_TO_EDGE, BasicTexture2D::CLAMP_TO_EDGE
		, 0, BasicTexture2D::RGBA, Game::Get().get_window_size()[0], Game::Get().get_window_size()[1], BasicTexture2D::FORMAT_RGBA, BasicTexture2D::FLOAT, nullptr)
	, m_fbo()
	, m_rbo(RenderBuffer::DEPTH_COMPONENT, Game::Get().get_window_size()[0], Game::Get().get_window_size()[1])
	, m_camera(nullptr)
	, m_shadow_map_shader(new Shader("ShadowMap.vert", "ShadowMap.frag"))
	, m_skybox_shader(new Shader("SkyBox.vert", "SkyBox.frag"))
	, m_normal_shader(new Shader("Normal.vert", "Normal.frag", "Normal.geom"))
	, m_light_shader(new Shader("Light.vert", "Light.frag"))
	, m_shader(new Shader("Phong.vert", "Blinn-Phong.frag"))
	, m_basic_texture_shader(new Shader("BasicShader.vert", "DrawTexture.frag"))
	, m_renderer(new Renderer3D())
	, m_fov(60.0f)
	, m_persp_matrix(Matrix4x4f::make_perspective_matrix(60.0f, Game::Get().get_window_aspect_ratio(), 0.01f))
	, m_skybox(nullptr)
	, m_ambinet_light()
	, m_flags(flags_draw_skybox)
	, m_current_shader(BLINN_PHONG)
{
	m_fbo.bind();
	m_fbo.attach_texture(m_texture, FrameBuffer::COLOR_ATTACHMENT0);
	m_fbo.attach_rbo(m_rbo, FrameBuffer::DEPTH_ATTACHMENT);
	m_fbo.check_status();
	m_fbo.unbind();
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
	if (!m_renderer->has_init())
		m_renderer->init(); // Ideally set it as active just when changing current project.

	if (m_camera != nullptr)
		m_camera->update();
	else
	{
		std::cout << "There isn't any camera" << std::endl;
		return; // Doen't continue if m_camera = nullptr 
	}
	
	m_renderer->clear(); // Clear default framebuffer

	draw_shadow_maps();
	m_fbo.bind();
	m_renderer->clear(Vector4f(0.1f, 0.1f, 0.1f, 1.0f)); // Clear default framebuffer
	draw_objects();
	if (m_flags & flags_draw_normals)
		draw_normals();
	if (m_flags & flags_draw_lights)
		draw_lights();
	if (m_flags & flags_draw_skybox)
		draw_skybox();
	m_fbo.unbind();

	// Post-Processing
	for (auto& aux : m_filters)
		aux->apply_filter(m_texture);

	static int const loc = m_basic_texture_shader->get_uniform_location("u_texture");
	m_basic_texture_shader->bind();
	m_texture.bind(0);
	m_basic_texture_shader->set_uniform1i(loc, 0);
	Renderer3D::draw_quad();
	m_basic_texture_shader->unbind();
	m_texture.unbind();
}

// It needs improvement (remove code repretition)
void ProjectController::imgui_renderer()
{
	m_camera->imgui_renderer();

	static bool* models_p_open;
	static bool* scene_p_open;
	if (ImGui::Begin("Models", models_p_open, ImGuiWindowFlags_MenuBar))
	{
		// Left
		static std::size_t selected = 0;
		ImGui::BeginChild("left pane", ImVec2(120, 0), true);

		{
			char label[128];
			sprintf(label, "Ambient Light");
			if (ImGui::Selectable(label, selected == 0))
				selected = 0;
		}

		// Lights
		for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		{
			char label[128];
			sprintf(label, "Light %d", i);
			if (ImGui::Selectable(label, selected == (i + 1)))
				selected = i + 1;
		}

		// Objects
		for (std::size_t i = m_light_buffer.size(); i < (m_light_buffer.size() + m_object_buffer.size()); i++)
		{
			char label[128];
			sprintf(label, m_object_buffer[i - m_light_buffer.size()]->get_name().c_str());
			if (ImGui::Selectable(label, selected == i + 1))
				selected = i + 1;
		}
	
		ImGui::EndChild();
		ImGui::SameLine();
		
		// Right 
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

		if (selected == 0)
		{
			ImGui::Text("Ambient Light");
			ImGui::Separator();
			m_ambinet_light.imgui_renderer();
		}
		
		if (selected < (m_light_buffer.size() + 1) && selected >= 1) 
		{
			ImGui::Text("Light: %d", selected);
			ImGui::Separator();
			m_light_buffer[selected - 1]->imgui_renderer();
		}
		else if(selected >= (m_light_buffer.size() + 1) && (m_object_buffer.size() + m_light_buffer.size() + 1) >= selected)
		{
			ImGui::Text("Object: %d", selected - m_light_buffer.size());
			ImGui::Separator();
			m_object_buffer[selected - m_light_buffer.size() - 1]->imgui_renderer();
		}
		ImGui::EndChild();

		ImGui::SetCursorPos(ImVec2(0, 500.0f));
	}
	ImGui::End();

	if (ImGui::Begin("Project Controller", scene_p_open, ImGuiWindowFlags_MenuBar))
	{
		// skybox
		if (m_skybox)
			m_skybox->imgui_rederer();
		else
			if (ImGui::Button("Attach skybox"))
				set_skybox(new TextureCubMap(open_folder_browser()));

		// add item
		if (ImGui::BeginMenu("Add Entity"))
		{
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Point Light"))
					create_light(0);
				if (ImGui::MenuItem("Spot Light"))
					create_light(1);
				if (ImGui::MenuItem("Directional Light"))
					create_light(2);
				if (ImGui::MenuItem("Shadow Caster Directional Light"))
					create_light(3);
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
					create_object(open_file_browser(L"Model Files", L"*.obj;*.dae;*.fbx;*.bvh;*.ply;*.stl"));
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		static bool post_processing_managemenet = false; 
		if (ImGui::CollapsingHeader("Post Processing", nullptr, ImGuiTreeNodeFlags_None))
		{
			ImGui::Indent(10.0f);
			if (ImGui::BeginMenu("Add Filter"))
			{
				static int is_actived = 0;
				if (ImGui::MenuItem("Gaussian Blur"))
				{
					if ((is_actived & filters_gaussian_blur) != filters_gaussian_blur)
					{
						is_actived |= filters_gaussian_blur;
						m_filters.push_back(new GaussianBlur());
						post_processing_managemenet = true;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Unindent(10.0f);
		}
		
		// Creat window to manage post processing
		if (post_processing_managemenet)
		{
			if (ImGui::Begin("Post Processing manegement", &post_processing_managemenet, ImGuiWindowFlags_None))
			{
				for (auto& aux : m_filters)
				{
					aux->imgui_renderer();
				}
			}
			ImGui::End();
		}

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
	m_persp_matrix = Matrix4x4f::make_perspective_matrix(m_fov, Game::Get().get_window_aspect_ratio(), 0.01f);
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
	case 3:
		push_light(new DirectionalLight({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {0.0f, -1.0f, 0.0f}, new ShadowMap(m_camera.get())));
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
	switch (m_current_camera)
	{
	case EDIT_CAMERA:
	{
		Vector3f dir = dynamic_cast<FPSCamera const*>(m_camera.get())->get_direction();
		m_camera.reset(new EditCamera(pos, pos + 5*dir, m_camera->get_yaw(), m_camera->get_pitch()));
		break;
	}
	case FPS_CAMERA:
	{
		m_camera.reset(new FPSCamera(m_camera.get()));
		break;
	}
	}
}

void ProjectController::draw_shadow_maps()
{
	m_shadow_map_shader->bind();
	for (auto& light : m_light_buffer)
	{
		if (!light->get_shadow_map())
			continue;
		light->get_shadow_map()->begin(); // bind framebuffer and change viewport size
		m_renderer->clear();
		m_shadow_map_shader->set_uniformMatrix4f(
			m_shadow_map_shader->get_uniform_location("u_light_view"), light->get_light_view());
		m_shadow_map_shader->set_uniformMatrix4f(
			m_shadow_map_shader->get_uniform_location("u_light_persp"), light->get_light_persp());
		for (auto& model : m_object_buffer)
		{
			if (model->is_visible())
			{
				Matrix4x4f model_matrix =
					model->get_scale_matrix() *
					model->get_rotation_matrix() *
					model->get_translation(); // Model view projection.
				m_shadow_map_shader->set_uniformMatrix4f(m_shadow_map_shader->get_uniform_location("u_model"), model_matrix);
		
				for (auto& mesh : model->get_meshes())
				{
					mesh->get_vertex_array().bind();
					mesh->get_index_buffer().bind();
					m_renderer->draw_element(mesh->get_index_buffer());
					mesh->get_vertex_array().unbind();
					mesh->get_index_buffer().unbind();
				}
			}
		}
		light->get_shadow_map()->end();
	}
	m_shadow_map_shader->unbind();
}

void ProjectController::draw_objects()
{
	m_shader->bind();

	// General uniforms.
	m_shader->set_uniform1ui(m_shader->get_uniform_location("u_NUM_POINT_LIGHT"), PointLight::get_count());
	m_shader->set_uniform1ui(m_shader->get_uniform_location("u_NUM_SPOT_LIGHT"), SpotLight::get_count());
	m_shader->set_uniform1ui(m_shader->get_uniform_location("u_NUM_DIRECTIONAL_LIGHT"), DirectionalLight::get_count());
	m_shader->set_uniform1ui(m_shader->get_uniform_location(
		"u_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH"), DirectionalLight::get_count_shadow_caster());
	m_shader->set_uniform3f(m_shader->get_uniform_location("u_ambient_light.color"), m_ambinet_light.get_light_color());
	m_shader->set_uniform1f(m_shader->get_uniform_location("u_ambient_light.strength"), m_ambinet_light.get_strength());

	m_shader->set_uniform3f(m_shader->get_uniform_location("u_view_pos"), m_camera->get_position());
	m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_view"), m_camera->get_view());
	m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_projection"), m_persp_matrix);

	// Light uniforms
	for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		m_light_buffer[i]->set_uniform(m_shader.get());

	for (auto& model : m_object_buffer)
	{
		if (model->is_visible())
		{
			Matrix4x4f model_matrix =
				model->get_scale_matrix() *
				model->get_rotation_matrix() *
				model->get_translation(); // Model view projection.
			m_shader->set_uniformMatrix4f(m_shader->get_uniform_location("u_model"), model_matrix);
			m_shader->set_uniform2f(m_shader->get_uniform_location("u_scale_uv"),
				model->get_material()->get_diffuse()->get_scale_uv());

			model->get_material()->get_diffuse()->bind(0);
			model->get_material()->get_specular()->bind(1);
			model->get_material()->get_normal_map()->bind(2);
			m_shader->set_uniform1i(m_shader->get_uniform_location("u_texture"), 0);
			m_shader->set_uniform1i(m_shader->get_uniform_location("u_specular_map"), 1);
			m_shader->set_uniform1i(m_shader->get_uniform_location("u_nomal_map"), 2);
			m_shader->set_uniform1i(m_shader->get_uniform_location("u_is_using_normal_map"),
				model->get_material()->is_using_normal_map());
			m_shader->set_uniform1f(m_shader->get_uniform_location("u_shininess"),
				model->get_material()->get_shininess());
			for (auto& mesh : model->get_meshes())
			{
				mesh->get_vertex_array().bind();
				mesh->get_index_buffer().bind();
				m_renderer->draw_element(mesh->get_index_buffer());
				mesh->get_vertex_array().unbind();
				mesh->get_index_buffer().unbind();
			}
			model->get_material()->get_diffuse()->unbind();
		}
	}
	m_shader->unbind();
}

void ProjectController::draw_normals()
{
	m_normal_shader->bind();
	m_normal_shader->set_uniformMatrix4f(m_normal_shader->get_uniform_location("u_view"), m_camera->get_view());
	m_normal_shader->set_uniformMatrix4f(m_normal_shader->get_uniform_location("u_projection"), m_persp_matrix);
	for (auto& model : m_object_buffer)
	{
		if (model->is_visible())
		{
			Matrix4x4f model_matrix =
				model->get_scale_matrix() *
				model->get_rotation_matrix() *
				model->get_translation(); //Model view projection
			m_shader->set_uniformMatrix4f(m_normal_shader->get_uniform_location("u_model"), model_matrix);

			for (auto& mesh : model->get_meshes())
			{
				mesh->get_vertex_array().bind();
				mesh->get_index_buffer().bind();
				m_renderer->draw_element(mesh->get_index_buffer());
				mesh->get_vertex_array().unbind();
				mesh->get_index_buffer().unbind();
			}
		}
	}
	m_normal_shader->unbind();
}

void ProjectController::draw_lights()
{
	m_light_shader->bind();
	m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_view"), m_camera->get_view());
	m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_projection"), m_persp_matrix);
	for (auto& light : m_light_buffer)
	{
		if (light->get_model()->is_visible())
		{
			Matrix4x4f model_matrix =
				light->get_model()->get_scale_matrix() *
				light->get_model()->get_rotation_matrix() *
				light->get_model()->get_translation(); // Model view projection
			m_light_shader->set_uniformMatrix4f(m_light_shader->get_uniform_location("u_model"), model_matrix);
			m_light_shader->set_uniform3f(m_light_shader->get_uniform_location("u_Color"), light->get_light_color());

			for (auto& mesh : light->get_model()->get_meshes())
			{
				mesh->get_vertex_array().bind();
				mesh->get_index_buffer().bind();
				m_renderer->draw_element(mesh->get_index_buffer());
				mesh->get_vertex_array().unbind();
				mesh->get_index_buffer().unbind();
			}
		}
	}
	m_light_shader->unbind();
}

void ProjectController::draw_skybox()
{
	if (m_skybox)
	{
		m_skybox->begin();
		m_skybox_shader->bind();

		m_skybox_shader->set_uniformMatrix4f(m_skybox_shader->get_uniform_location("u_projection"), m_persp_matrix);
		m_skybox_shader->set_uniformMatrix4f(m_skybox_shader->get_uniform_location("u_view"), m_camera->get_view());
		m_skybox_shader->set_uniform3f(m_skybox_shader->get_uniform_location("u_camera_position"), m_camera->get_position()); // this is temporary.
		m_skybox_shader->set_uniform1i(m_skybox_shader->get_uniform_location("u_skybox"), 0);
		m_skybox->draw();
		m_skybox_shader->unbind();
		m_skybox->end();
	}
}
