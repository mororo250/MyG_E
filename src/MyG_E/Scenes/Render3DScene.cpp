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
	mBuffer.reserve(8);
	for (unsigned int i = 0; i < mBuffer.capacity()/2; i++)
		mBuffer.push_back(cube);
	for (unsigned int i = mBuffer.capacity() / 2; i < mBuffer.capacity(); i++)
		mBuffer.push_back(sphere);
	{		 
		mBuffer[0].SetPosition({0.0f, 0.0f, -3.0f});
		mBuffer[1].SetPosition({ 1.5f, 0.0f, -3.0f });
		mBuffer[2].SetPosition({ 0.0f, 1.5f, -3.0f });
		mBuffer[3].SetPosition({ 1.5f, 1.5f, -3.0f });
		mBuffer[4].SetPosition({ 0.0f, 0.0f, 3.0f });
		mBuffer[5].SetPosition({ 1.5f, 0.0f, 3.0f });
		mBuffer[6].SetPosition({ 0.0f, 1.5f, 3.0f });
		mBuffer[7].SetPosition({ 1.5f, 1.5f, 3.0f });

		mBuffer[4].SetScale({0.05f, 0.05f, 0.05f});
		mBuffer[5].SetScale({0.05f, 0.05f, 0.05f});
		mBuffer[6].SetScale({0.05f, 0.05f, 0.05f});
		mBuffer[7].SetScale({0.05f, 0.05f, 0.05f});

		mBuffer[0].SetMaterial(esmerald);
		mBuffer[1].SetMaterial(jade);
		mBuffer[2].SetMaterial(obsidian);
		mBuffer[3].SetMaterial(pearl);
		mBuffer[4].SetMaterial(esmerald);
		mBuffer[5].SetMaterial(jade);
		mBuffer[6].SetMaterial(obsidian);
		mBuffer[7].SetMaterial(pearl);
	}
	
	for (auto& I : mBuffer)
		mListboxItem.push_back(I.GetObjectName().c_str());
	
	mFPSCamera = std::make_unique<FPSCamera>(Vector<float, 3>({ 0.0f, 0.0f, 0.0f }), Vector<float, 3>({0.0f, 0.0f, -1.0f}));
	mEditCamera = std::make_unique<EditCamera>(Vector<float, 3>({ 0.0f, 0.0f, 5.0f }), Vector<float, 3>({ 0.0f, 0.0f, 0.0f }));
	
	mLight = std::make_unique<Light>(Vector<float, 3>({ 1.0f, 1.0f, 0.0f }));

	GLcall(glEnable(GL_DEPTH_TEST));
	

	mLightShader = std::make_unique<Shader>("Light.glsl");
	mShader = std::make_unique<Shader>("Phong.glsl");
	mShader->bind();

	m_u_Model = mShader->GetUniformLocation("u_Model");
	m_u_ViewProjection = mShader->GetUniformLocation("u_ViewProjection");

	mShader->unbind();

	mRenderer = std::make_unique<Renderer>();
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
	mLight->ImGuiRenderer();
	ImGui::ListBox("ObjectList", &current_object_id, mListboxItem.data(), mListboxItem.size(), 3);	

	// Create indenpendent ImGui window for selected object
	if (current_object_id != -1)
	{
		ImGui::Begin("Object x");

		mBuffer[current_object_id].ImGuiRenderer();

		if (ImGui::Button("Point Camera"))
			mEditCamera->SetFocalPoint(mBuffer[current_object_id].GetPosition());

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
	mShader->SetUniform3f(mShader->GetUniformLocation("u_Light.color"), mLight->GetLightColor());
	mShader->SetUniform3f(mShader->GetUniformLocation("u_Light.position"), mLight->GetLightPosition());
	mShader->SetUniform1f(mShader->GetUniformLocation("u_Light.ambient_strength"), mLight->GetAmbientStength());
	mShader->SetUniform1f(mShader->GetUniformLocation("u_Light.diffuse_strength"), mLight->GetDiffuseStrength());
	mShader->SetUniform1f(mShader->GetUniformLocation("u_Light.specular_strength"), mLight->GetSpecularStrength());
	mShader->SetUniform3f(mShader->GetUniformLocation("u_ViewPos"), mEditCamera->GetPosition());

	for (auto& aux : mBuffer)
	{
		if (aux.isVisible())
		{
			mModel = aux.GetScale() * aux.GetRotation() * aux.GetTranslation(); //Model view projection
			mViewProjection = mEditCamera->GetView() * mPersp;

			mShader->SetUniformMatrix4f(m_u_Model, mModel);
			mShader->SetUniformMatrix4f(m_u_ViewProjection, mViewProjection);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.ambient"), aux.GetMaterial().ambient);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.diffuse"), aux.GetMaterial().diffuse);
			mShader->SetUniform3f(mShader->GetUniformLocation("u_Material.specular"), aux.GetMaterial().specular);
			mShader->SetUniform1f(mShader->GetUniformLocation("u_Material.shininess"), aux.GetMaterial().shininess);

			aux.GetMesh()->GetVertexArray().bind();
			aux.GetMesh()->GetIndexBuffer().bind();
			mRenderer->Draw(aux.GetMesh()->GetIndexBuffer());
			aux.GetMesh()->GetVertexArray().unbind();
			aux.GetMesh()->GetIndexBuffer().unbind();
		}
	}

	// Light
	{
		mLightShader->bind();
		Model3D aux = mLight->GetModel();
		if (aux.isVisible())
		{
			mModel = aux.GetScale() * aux.GetRotation() * aux.GetTranslation(); //Model view projection
			mViewProjection = mEditCamera->GetView() * mPersp;

			mLightShader->SetUniformMatrix4f(mLightShader->GetUniformLocation("u_Model"), mModel);
			mLightShader->SetUniformMatrix4f(mLightShader->GetUniformLocation("u_ViewProjection"), mViewProjection);
			mLightShader->SetUniform3f(mLightShader->GetUniformLocation("u_Color"), mLight->GetLightColor());

			aux.GetMesh()->GetVertexArray().bind();
			aux.GetMesh()->GetIndexBuffer().bind();
			mRenderer->Draw(aux.GetMesh()->GetIndexBuffer());
			aux.GetMesh()->GetVertexArray().unbind();
			aux.GetMesh()->GetIndexBuffer().unbind();
		}
	}
	mShader->unbind();
}
