#include "MasterController.h"

MasterController::MasterController()
{
	m_light_shader = std::make_unique<Shader>("Light.glsl");
	m_shader = std::make_unique<Shader>("Phong.glsl");
	m_renderer = std::make_unique<Renderer3D>();
}

MasterController::~MasterController()
{
}

void MasterController::Update()
{
	m_camera->Update();

	m_renderer->Clear();

	m_shader->bind();

	for (unsigned int i = 0; i < m_light_list.size(); i++)
		m_light_list[i]->SetUniform(m_shader.get());

	// General uniforms.
	m_shader->SetUniform1i(m_shader->GetUniformLocation("u_NumPointLight"), 2);
	m_shader->SetUniform1i(m_shader->GetUniformLocation("u_NumSpotLight"), 2);
	m_shader->SetUniform1i(m_shader->GetUniformLocation("u_NumDirectionalLight"), 1);
	m_shader->SetUniform3f(m_shader->GetUniformLocation("u_ViewPos"), m_camera->GetPosition());

	for (auto& aux : m_buffer)
	{
		if (aux.isVisible())
		{
			Matrix<float, 4, 4> model_matrix = aux.GetScale() * aux.GetRotation() * aux.GetTranslation(); //Model view projection
			Matrix<float, 4, 4> view_projection = m_camera->GetView() * m_persp_matrix;

			// Materials uniforms.
			m_shader->SetUniformMatrix4f(m_shader->GetUniformLocation("u_Model"), model_matrix);
			m_shader->SetUniformMatrix4f(m_shader->GetUniformLocation("u_ViewProjection"), view_projection);
			m_shader->SetUniform3f(m_shader->GetUniformLocation("u_Material.ambient"), aux.GetMaterial().ambient);
			m_shader->SetUniform3f(m_shader->GetUniformLocation("u_Material.diffuse"), aux.GetMaterial().diffuse);
			m_shader->SetUniform3f(m_shader->GetUniformLocation("u_Material.specular"), aux.GetMaterial().specular);
			m_shader->SetUniform1f(m_shader->GetUniformLocation("u_Material.shininess"), aux.GetMaterial().shininess);

			aux.GetMesh()->GetVertexArray().bind();
			aux.GetMesh()->GetIndexBuffer().bind();
			m_renderer->Draw(aux.GetMesh()->GetIndexBuffer());
			aux.GetMesh()->GetVertexArray().unbind();
			aux.GetMesh()->GetIndexBuffer().unbind();
		}
	}
	m_shader->unbind();

	// Light
	{
		m_light_shader->bind();
		for (unsigned int i = 0; i < m_light_list.size(); i++)
		{
			Model3D light_model = m_light_list[i]->GetModel();
			if (light_model.isVisible())
			{
				Matrix<float, 4, 4> model_matrix = light_model.GetScale() * light_model.GetRotation() * light_model.GetTranslation(); //Model view projection
				Matrix<float, 4, 4> view_projection = m_camera->GetView() * m_persp_matrix;

				m_light_shader->SetUniformMatrix4f(m_light_shader->GetUniformLocation("u_Model"), model_matrix);
				m_light_shader->SetUniformMatrix4f(m_light_shader->GetUniformLocation("u_ViewProjection"), view_projection);
				m_light_shader->SetUniform3f(m_light_shader->GetUniformLocation("u_Color"), m_light_list[i]->GetLightColor());

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
