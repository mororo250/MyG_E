#include "Render3DScene.h"
#include "Foundation/Input.h"
#include "imgui.h"
#include <time.h>

#include <string>


// There is a big problem here
// If we push a new item to ligttPoint or direction_light the light_list pointer become invalid
Render3DScene::Render3DScene()
	: mPersp(CreatePerspectiveMatrix(45.0f, 1024.0f / 768.0f, 0.1f, 800.0f))
{
	Mesh* plane(new Mesh(Shape::PLANE));

	Material esmerald({0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6);
	
	// Create the models
	m_object_buffer.push_back(new Model3D(plane));
	delete plane;
	{		 
		m_object_buffer[0]->SetPosition({ 0.0f, -1.0f, 0.0f });
		m_object_buffer[0]->SetScale({ 100.0f, 0.0f, 100.0f });
		m_object_buffer[0]->SetMaterial(esmerald);
		
	}
	for (auto& aux : m_object_buffer)
		mListboxItem.push_back(aux->GetObjectName().c_str());
	
	// Create Lights
	{
		m_light_buffer.push_back(new PointLight(Vector<float, 3>({ 1.0f, 1.0f, 0.0f })));
		m_light_buffer[0]->SetAmbientStength(0.0f);
		m_light_buffer.push_back(new DirectionalLight(Vector<float, 3>({ 1.0f, 1.0f, 0.0f })));
	}

	// Create Camera
	mEditCamera = new EditCamera(Vector<float, 3>({ 0.0f, 0.0f, 5.0f }), Vector<float, 3>({ 0.0f, 0.0f, 0.0f }));
	

	mLightShader = std::make_unique<Shader>("Light.glsl");
	mShader = std::make_unique<Shader>("Phong.glsl");
	m_renderer = std::make_unique<Renderer3D>();
}

Render3DScene::~Render3DScene()
{
	for (auto& aux : m_light_buffer)
		delete aux;
}

void Render3DScene::ImGuiRenderer()
{
	mEditCamera->ImGuiRenderer();

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

void Render3DScene::Update()
{
	mEditCamera->Update();

	m_renderer->Clear();
	
	mShader->bind();

	for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		m_light_buffer[i]->SetUniform(mShader.get());

	// General uniforms.
	mShader->SetUniform1i(mShader->GetUniformLocation("u_NumPointLight"), PointLight::get_count());
	mShader->SetUniform1i(mShader->GetUniformLocation("u_NumSpotLight"), PointLight::get_count());
	mShader->SetUniform1i(mShader->GetUniformLocation("u_NumDirectionalLight"), PointLight::get_count());
	mShader->SetUniform3f(mShader->GetUniformLocation("u_ViewPos"), mEditCamera->GetPosition());

	for (auto& aux : m_object_buffer)
	{
		if (aux->isVisible())
		{
			m_model = aux->GetScale() * aux->GetRotation() * aux->GetTranslation(); //Model view projection
			mViewProjection = mEditCamera->GetView() * mPersp;
			
			// Materials uniforms.
			mShader->SetUniformMatrix4f(mShader->GetUniformLocation("u_Model"), m_model);
			mShader->SetUniformMatrix4f(mShader->GetUniformLocation("u_ViewProjection"), mViewProjection);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.ambient"), aux->GetMaterial().ambient);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.diffuse"), aux->GetMaterial().diffuse);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.specular"), aux->GetMaterial().specular);
			mShader->SetUniform1f(mShader->GetUniformLocation("u_Material.shininess"), aux->GetMaterial().shininess);

			aux->GetMesh()->GetVertexArray().bind();
			aux->GetMesh()->GetIndexBuffer().bind();
			m_renderer->Draw(aux->GetMesh()->GetIndexBuffer());
			aux->GetMesh()->GetVertexArray().unbind();
			aux->GetMesh()->GetIndexBuffer().unbind();
		}
	}
	mShader->unbind();

	// Light
	{
		mLightShader->bind();
		for (unsigned int i = 0; i < m_light_buffer.size(); i++)
		{
			Model3D* light_model = m_light_buffer[i]->GetModel();
			if (light_model->isVisible())
			{
				m_model = light_model->GetScale() * light_model->GetRotation() * light_model->GetTranslation(); //Model view projection
				mViewProjection = mEditCamera->GetView() * mPersp;

				mLightShader->SetUniformMatrix4f(mLightShader->GetUniformLocation("u_Model"), m_model);
				mLightShader->SetUniformMatrix4f(mLightShader->GetUniformLocation("u_ViewProjection"), mViewProjection);
				mLightShader->SetUniform3f(mLightShader->GetUniformLocation("u_Color"), m_light_buffer[i]->GetLightColor());

				light_model->GetMesh()->GetVertexArray().bind();
				light_model->GetMesh()->GetIndexBuffer().bind();
				m_renderer->Draw(light_model->GetMesh()->GetIndexBuffer());
				light_model->GetMesh()->GetVertexArray().unbind();
				light_model->GetMesh()->GetIndexBuffer().unbind();
			}
		}
		mLightShader->unbind();
	}
}
