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

	for (auto& aux : m_object_buffer)
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
		for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		{
			Model3D light_model = m_light_buffer[i]->GetModel();
			if (light_model.isVisible())
			{
				Matrix<float, 4, 4> model_matrix = light_model.GetScale() * light_model.GetRotation() * light_model.GetTranslation(); //Model view projection
				Matrix<float, 4, 4> view_projection = m_camera->GetView() * m_persp_matrix;

				m_light_shader->SetUniformMatrix4f(m_light_shader->GetUniformLocation("u_Model"), model_matrix);
				m_light_shader->SetUniformMatrix4f(m_light_shader->GetUniformLocation("u_ViewProjection"), view_projection);
				m_light_shader->SetUniform3f(m_light_shader->GetUniformLocation("u_Color"), m_light_buffer[i]->GetLightColor());

				light_model.GetMesh()->GetVertexArray().bind();
				light_model.GetMesh()->GetIndexBuffer().bind();
				m_renderer->Draw(light_model.GetMesh()->GetIndexBuffer());
				light_model.GetMesh()->GetVertexArray().unbind();
				light_model.GetMesh()->GetIndexBuffer().unbind();
			}
		}
		m_light_shader->unbind();
	}
}

void ProjectController::imgui_renderer()
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Example: Property editor"))
	{
		ImGui::End();
		return;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::Columns(2);
	ImGui::Separator();

	struct funcs
	{
		static void ShowDummyObject(const char* prefix, int uid)
		{
			ImGui::PushID(uid);						// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
			ImGui::AlignTextToFramePadding();		// Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
			bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
			ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("my sailor is rich");
			ImGui::NextColumn();
			if (node_open)
			{
				static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
				for (int i = 0; i < 8; i++)
				{
					ImGui::PushID(i);				// Use field index as identifier.
					if (i < 2)
					{
						ShowDummyObject("Child", 424242);
					}
					else
					{
						// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						if (i >= 5)
							ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
						else
							ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
						ImGui::NextColumn();
					}
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	};

	// Iterate dummy objects with dummy members (all the same data)
	for (int obj_i = 0; obj_i < 3; obj_i++)
		funcs::ShowDummyObject("Object", obj_i);

	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::PopStyleVar();
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
