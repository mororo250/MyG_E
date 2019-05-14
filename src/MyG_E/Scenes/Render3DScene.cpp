#include "Render3DScene.h"
#include "Foundation/Input.h"
#include "imgui.h"
#include <time.h>

#include <string>

Render3DScene::Render3DScene()
	: mPersp(CreatePerspectiveMatrix(45.0f, 1024.0f / 768.0f, 0.1f, 800.0f))
{
	srand(time(NULL));
	
	std::shared_ptr<Mesh> cube(new Mesh(Shape::CUBE));
	std::shared_ptr<Mesh> sphere(new Mesh(Shape::SPHERE));

	Material esmerald({0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6);
	Material jade({0.135, 0.2225, 0.1575}, {0.54, 0.89, 0.63}, {0.316228, 0.316228, 0.316228 }, 0.1);
	Material obsidian({0.05375, 0.05, 0.06625}, {0.18275, 0.17, 0.22525}, {0.332741, 0.328634, 0.3464351}, 0.3);
	Material pearl({0.25, 0.20725, 0.20725}, {1.0, 0.829, 0.829}, {0.296648, 0.296648, 0.296648}, 0.088);
	
	// Create the models
	m_buffer.reserve(8);
	for (unsigned int i = 0; i < m_buffer.capacity()/2; i++)
		m_buffer.push_back(cube);
	for (unsigned int i = m_buffer.capacity() / 2; i < m_buffer.capacity(); i++)
		m_buffer.push_back(sphere);
	{		 
		m_buffer[0].SetPosition({0.0f, 0.0f, -3.0f});
		m_buffer[1].SetPosition({ 1.5f, 0.0f, -3.0f });
		m_buffer[2].SetPosition({ 0.0f, 1.5f, -3.0f });
		m_buffer[3].SetPosition({ 1.5f, 1.5f, -3.0f });
		m_buffer[4].SetPosition({ 0.0f, 0.0f, 3.0f });
		m_buffer[5].SetPosition({ 1.5f, 0.0f, 3.0f });
		m_buffer[6].SetPosition({ 0.0f, 1.5f, 3.0f });
		m_buffer[7].SetPosition({ 1.5f, 1.5f, 3.0f });

		m_buffer[4].SetScale({0.05f, 0.05f, 0.05f});
		m_buffer[5].SetScale({0.05f, 0.05f, 0.05f});
		m_buffer[6].SetScale({0.05f, 0.05f, 0.05f});
		m_buffer[7].SetScale({0.05f, 0.05f, 0.05f});

		m_buffer[0].SetMaterial(esmerald);
		m_buffer[1].SetMaterial(jade);
		m_buffer[2].SetMaterial(obsidian);
		m_buffer[3].SetMaterial(pearl);
		m_buffer[4].SetMaterial(esmerald);
		m_buffer[5].SetMaterial(jade);
		m_buffer[6].SetMaterial(obsidian);
		m_buffer[7].SetMaterial(pearl);
	}
	for (auto& I : m_buffer)
		mListboxItem.push_back(I.GetObjectName().c_str());

	{
		mLight.push_back(Vector<float, 3>({ 1.0f, 1.0f, 0.0f }));
		mLight.push_back(Vector<float, 3>({ 1.0f, 10.0f, 10.0f }));
		mLight.push_back(Vector<float, 3>({ 1.0f, 10.0f, -10.0f }));
	}

	mFPSCamera = std::make_unique<FPSCamera>(Vector<float, 3>({ 0.0f, 0.0f, 0.0f }), Vector<float, 3>({0.0f, 0.0f, -1.0f}));
	mEditCamera = std::make_unique<EditCamera>(Vector<float, 3>({ 0.0f, 0.0f, 5.0f }), Vector<float, 3>({ 0.0f, 0.0f, 0.0f }));
	

	GLcall(glEnable(GL_DEPTH_TEST));
	

	mLightShader = std::make_unique<Shader>("Light.glsl");
	mShader = std::make_unique<Shader>("Phong.glsl");
	mShader->bind();

	m_u_Model = mShader->GetUniformLocation("u_Model");
	m_u_ViewProjection = mShader->GetUniformLocation("u_ViewProjection");

	mShader->unbind();

	m_renderer = std::make_unique<Renderer>();
}

Render3DScene::~Render3DScene()
{
	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLcall(glDisable(GL_DEPTH_TEST));
}

void Render3DScene::ImGuiRenderer()
{
	static int current_object_id = -1;

	mEditCamera->ImGuiRenderer();
	
	for (auto& aux : mLight)
		aux.ImGuiRenderer();

	ImGui::ListBox("ObjectList", &current_object_id, mListboxItem.data(), mListboxItem.size(), 3);	

	// Create indenpendent ImGui window for selected object
	if (current_object_id != -1)
	{
		ImGui::Begin("Object x");

		m_buffer[current_object_id].ImGuiRenderer();

		if (ImGui::Button("Point Camera"))
			mEditCamera->SetFocalPoint(m_buffer[current_object_id].GetPosition());

		if (ImGui::Button("deselect"))
			current_object_id = -1;
		
		ImGui::End();
	}
}

void Render3DScene::Update()
{
	mEditCamera->Update();

	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	mShader->bind();
	for (unsigned int i = 0; i < mLight.size(); i++ )
	{
		mShader->SetUniform3f(mShader->GetUniformLocation("u_Light[" + std::to_string(i) + "].color"), mLight[i].GetLightColor());
		mShader->SetUniform3f(mShader->GetUniformLocation("u_Light[" + std::to_string(i) + "].position"), mLight[i].GetLightPosition());
		mShader->SetUniform1f(mShader->GetUniformLocation("u_Light[" + std::to_string(i) + "].ambient_strength"), mLight[i].GetAmbientStength());
		mShader->SetUniform1f(mShader->GetUniformLocation("u_Light[" + std::to_string(i) + "].diffuse_strength"), mLight[i].GetDiffuseStrength());
		mShader->SetUniform1f(mShader->GetUniformLocation("u_Light[" + std::to_string(i) + "].specular_strength"), mLight[i].GetSpecularStrength());
	}
	mShader->SetUniform1i(mShader->GetUniformLocation("u_NumLight"), mLight.size());
	mShader->SetUniform3f(mShader->GetUniformLocation("u_ViewPos"), mEditCamera->GetPosition());

	for (auto& aux : m_buffer)
	{
		if (aux.isVisible())
		{
			m_model = aux.GetScale() * aux.GetRotation() * aux.GetTranslation(); //Model view projection
			mViewProjection = mEditCamera->GetView() * mPersp;

			mShader->SetUniformMatrix4f(m_u_Model, m_model);
			mShader->SetUniformMatrix4f(m_u_ViewProjection, mViewProjection);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.ambient"), aux.GetMaterial().ambient);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.diffuse"), aux.GetMaterial().diffuse);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.specular"), aux.GetMaterial().specular);
			mShader->SetUniform1f(mShader->GetUniformLocation("u_Material.shininess"), aux.GetMaterial().shininess);

			aux.GetMesh()->GetVertexArray().bind();
			aux.GetMesh()->GetIndexBuffer().bind();
			m_renderer->Draw(aux.GetMesh()->GetIndexBuffer());
			aux.GetMesh()->GetVertexArray().unbind();
			aux.GetMesh()->GetIndexBuffer().unbind();
		}
	}

	// Light
	{
		mLightShader->bind();
		for (auto& aux : mLight)
		{
			Model3D light_model = aux.GetModel();
			if (light_model.isVisible())
			{
				m_model = light_model.GetScale() * light_model.GetRotation() * light_model.GetTranslation(); //Model view projection
				mViewProjection = mEditCamera->GetView() * mPersp;

				mLightShader->SetUniformMatrix4f(mLightShader->GetUniformLocation("u_Model"), m_model);
				mLightShader->SetUniformMatrix4f(mLightShader->GetUniformLocation("u_ViewProjection"), mViewProjection);
				mLightShader->SetUniform3f(mLightShader->GetUniformLocation("u_Color"), aux.GetLightColor());

				light_model.GetMesh()->GetVertexArray().bind();
				light_model.GetMesh()->GetIndexBuffer().bind();
				m_renderer->Draw(light_model.GetMesh()->GetIndexBuffer());
				light_model.GetMesh()->GetVertexArray().unbind();
				light_model.GetMesh()->GetIndexBuffer().unbind();
			}
		}
	}
	mShader->unbind();
}
