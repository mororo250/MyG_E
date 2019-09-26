#include "ProjectController.h"
#include "imgui.h"

ProjectController::ProjectController()
	:m_camera(nullptr),
	m_light_shader(new Shader("Light.glsl")),
	m_shader(new Shader("Phong.glsl")),
	m_renderer(new Renderer3D()),
	m_persp_matrix(CreatePerspectiveMatrix(45.0f, 1024.0f / 768.0f, 0.1f, 800.0f))
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
		m_camera->Update();
	else
	{
		std::cout << "There isn't any camera" << std::endl;
		return; // Doen't continue if m_camera = nullptr 
	}

	m_renderer->Clear();

	m_shader->bind();

	for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		m_light_buffer[i]->set_uniform(m_shader.get());

	// General uniforms.
	m_shader->set_uniform1i(m_shader->GetUniformLocation("u_NumPointLight"), PointLight::get_count());
	m_shader->set_uniform1i(m_shader->GetUniformLocation("u_NumSpotLight"), SpotLight::get_count());
	m_shader->set_uniform1i(m_shader->GetUniformLocation("u_NumDirectionalLight"), DirectionalLight::get_count());
	m_shader->set_uniform3f(m_shader->GetUniformLocation("u_ViewPos"), m_camera->GetPosition());

	for (auto* aux : m_object_buffer)
	{
		if (aux->is_visible())
		{
			Matrix<float, 4, 4> model_matrix = aux->get_scale_matrix() * aux->get_rotation_matrix() * aux->get_translation(); //Model view projection
			Matrix<float, 4, 4> view_projection = m_camera->GetView() * m_persp_matrix;

			// Materials uniforms.
			m_shader->set_uniformMatrix4f(m_shader->GetUniformLocation("u_Model"), model_matrix);
			m_shader->set_uniformMatrix4f(m_shader->GetUniformLocation("u_ViewProjection"), view_projection);
			m_shader->set_uniform3f(m_shader->GetUniformLocation("u_Material.ambient"), aux->get_material().ambient);
			m_shader->set_uniform3f(m_shader->GetUniformLocation("u_Material.diffuse"), aux->get_material().diffuse);
			m_shader->set_uniform3f(m_shader->GetUniformLocation("u_Material.specular"), aux->get_material().specular);
			m_shader->set_uniform1f(m_shader->GetUniformLocation("u_Material.shininess"), aux->get_material().shininess);

			aux->get_mesh()->GetVertexArray().bind();
			aux->get_mesh()->GetIndexBuffer().bind();
			m_renderer->Draw(aux->get_mesh()->GetIndexBuffer());
			aux->get_mesh()->GetVertexArray().unbind();
			aux->get_mesh()->GetIndexBuffer().unbind();
		}
	}
	m_shader->unbind();

	// Light
	{
		m_light_shader->bind();
		for (auto* aux : m_light_buffer)
		{
			if (aux->GetModel()->is_visible())
			{
				Matrix<float, 4, 4> model_matrix = aux->GetModel()->get_scale_matrix() * aux->GetModel()->get_rotation_matrix() * aux->GetModel()->get_translation(); //Model view projection
				Matrix<float, 4, 4> view_projection = m_camera->GetView() * m_persp_matrix;

				m_light_shader->set_uniformMatrix4f(m_light_shader->GetUniformLocation("u_Model"), model_matrix);
				m_light_shader->set_uniformMatrix4f(m_light_shader->GetUniformLocation("u_ViewProjection"), view_projection);
				m_light_shader->set_uniform3f(m_light_shader->GetUniformLocation("u_Color"), aux->GetLightColor());

				aux->GetModel()->get_mesh()->GetVertexArray().bind();
				aux->GetModel()->get_mesh()->GetIndexBuffer().bind();
				m_renderer->Draw(aux->GetModel()->get_mesh()->GetIndexBuffer());
				aux->GetModel()->get_mesh()->GetVertexArray().unbind();
				aux->GetModel()->get_mesh()->GetIndexBuffer().unbind();
			}
		}
		m_light_shader->unbind();
	}
}

// It need to improve (remove code repretition)
void ProjectController::imgui_renderer()
{
	m_camera->ImGuiRenderer();

	static bool* p_open;
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Editor", p_open, ImGuiWindowFlags_MenuBar))
	{

		// Left
		static int selected = 0;
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
		
		// Lights
		for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		{
			char label[128];
			sprintf(label, "Light %d", i);
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}

		// Objects
		for (unsigned int i = m_light_buffer.size(); i < (m_light_buffer.size() + m_object_buffer.size()); i++)
		{
			char label[128];
			sprintf(label, "object %d", i - m_light_buffer.size());
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}

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
					create_object(Shape::PLANE);
				if (ImGui::MenuItem("Cube"))
					create_object(Shape::CUBE);
				if (ImGui::MenuItem("Pyramid"))
					create_object(Shape::PYRAMID);
				if (ImGui::MenuItem("Sphere"))
					create_object(Shape::SPHERE);
				if (ImGui::MenuItem("Import Mesh"));
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		
		ImGui::EndChild();
		ImGui::SameLine();
		
		// Right 
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		
		if (selected < m_light_buffer.size()) {
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
		else
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

void ProjectController::create_object(Shape const shape)
{
	push_object(new Model3D(new Mesh(shape)));
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
