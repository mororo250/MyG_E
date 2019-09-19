#include "ProjectController.h"
#include "imgui.h"

ProjectController::ProjectController()
	:m_camera(nullptr),
	m_light_shader(new Shader("Light.glsl")),
	m_shader(new Shader("Phong.glsl")),
	m_renderer(new Renderer3D())
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
		std::cout << "" << std::endl;
		return; // Doen't continue if m_camera = nullptr 
	}

	m_renderer->Clear();

	m_shader->bind();

	for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		m_light_buffer[i]->SetUniform(m_shader.get());

	// General uniforms.
	m_shader->SetUniform1i(m_shader->GetUniformLocation("u_NumPointLight"), 2);
	m_shader->SetUniform1i(m_shader->GetUniformLocation("u_NumSpotLight"), 2);
	m_shader->SetUniform1i(m_shader->GetUniformLocation("u_NumDirectionalLight"), 1);
	m_shader->SetUniform3f(m_shader->GetUniformLocation("u_ViewPos"), m_camera->GetPosition());

	for (auto* aux : m_object_buffer)
	{
		if (aux->isVisible())
		{
			Matrix<float, 4, 4> model_matrix = aux->GetScale() * aux->GetRotation() * aux->GetTranslation(); //Model view projection
			Matrix<float, 4, 4> view_projection = m_camera->GetView() * m_persp_matrix;

			// Materials uniforms.
			m_shader->SetUniformMatrix4f(m_shader->GetUniformLocation("u_Model"), model_matrix);
			m_shader->SetUniformMatrix4f(m_shader->GetUniformLocation("u_ViewProjection"), view_projection);
			m_shader->SetUniform3f(m_shader->GetUniformLocation("u_Material.ambient"), aux->GetMaterial().ambient);
			m_shader->SetUniform3f(m_shader->GetUniformLocation("u_Material.diffuse"), aux->GetMaterial().diffuse);
			m_shader->SetUniform3f(m_shader->GetUniformLocation("u_Material.specular"), aux->GetMaterial().specular);
			m_shader->SetUniform1f(m_shader->GetUniformLocation("u_Material.shininess"), aux->GetMaterial().shininess);

			aux->GetMesh()->GetVertexArray().bind();
			aux->GetMesh()->GetIndexBuffer().bind();
			m_renderer->Draw(aux->GetMesh()->GetIndexBuffer());
			aux->GetMesh()->GetVertexArray().unbind();
			aux->GetMesh()->GetIndexBuffer().unbind();
		}
	}
	m_shader->unbind();

	// Light
	{
		m_light_shader->bind();
		for (auto* aux : m_light_buffer)
		{
			if (aux->GetModel().isVisible())
			{
				Matrix<float, 4, 4> model_matrix = aux->GetModel().GetScale() * aux->GetModel().GetRotation() * aux->GetModel().GetTranslation(); //Model view projection
				Matrix<float, 4, 4> view_projection = m_camera->GetView() * m_persp_matrix;

				m_light_shader->SetUniformMatrix4f(m_light_shader->GetUniformLocation("u_Model"), model_matrix);
				m_light_shader->SetUniformMatrix4f(m_light_shader->GetUniformLocation("u_ViewProjection"), view_projection);
				m_light_shader->SetUniform3f(m_light_shader->GetUniformLocation("u_Color"), aux->GetLightColor());

				aux->GetModel().GetMesh()->GetVertexArray().bind();
				aux->GetModel().GetMesh()->GetIndexBuffer().bind();
				m_renderer->Draw(aux->GetModel().GetMesh()->GetIndexBuffer());
				aux->GetModel().GetMesh()->GetVertexArray().unbind();
				aux->GetModel().GetMesh()->GetIndexBuffer().unbind();
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
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
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
