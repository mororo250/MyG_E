#include "Render3DScene.h"
#include "Foundation/Input.h"
#include "imgui.h"
#include <time.h>

#include <string>

Render3DScene::Render3DScene()
	:mPersp(CreatePerspectiveMatrix(45.0f, 1024.0f / 768.0f, 0.1f, 800.0f))
{
	srand(time(NULL));

	std::vector<Vector<float, 4>> color;
	
	std::vector<std::shared_ptr<Mesh>> mesh_buffer;
	mesh_buffer.reserve(17);
	
	for (unsigned int i = 0; i < mesh_buffer.capacity(); i++)
	{
		color.clear();
		for(unsigned int i = 0; i < 24; i++)
			color.push_back({ rand() % 100 / 100.0f,  rand() % 100 / 100.0f,  rand() % 100 / 100.0f ,  1.0 });
		mesh_buffer.push_back(std::make_shared<Mesh>(color));
	}
	
	// Create the models
	mBuffer.reserve(17);
	{
		mBuffer.push_back(mesh_buffer[0]);			 
		mBuffer[0].SetPosition({0.0f, 0.0f, -30.0f});
		mBuffer[0].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer.push_back(mesh_buffer[1]);	
		mBuffer[1].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[1].SetPosition({ 1.5f, 1.5f, -10.0f });
		mBuffer.push_back(mesh_buffer[2]);
		mBuffer[2].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[2].SetPosition({ -3.0f, 3.0f, -14.0f });
		mBuffer.push_back(mesh_buffer[3]);			 
		mBuffer[3].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[3].SetPosition({ 4.0f, -2.0f, -12.0f });
		mBuffer.push_back(mesh_buffer[4]);
		mBuffer[4].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[4].SetPosition({ -6.0f, -6.0f, -20.0f });
		mBuffer.push_back(mesh_buffer[5]);
		mBuffer[5].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[5].SetPosition({ 7.0f, 6.0f, -23.0f });
		mBuffer.push_back(mesh_buffer[6]);
		mBuffer[6].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[6].SetPosition({ -10.0f, 8.0f, -25.0f });
		mBuffer.push_back(mesh_buffer[7]);
		mBuffer[7].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[7].SetPosition({ -10.0f, -8.0f, -25.0f });
		mBuffer.push_back(mesh_buffer[8]);
		mBuffer[8].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[8].SetPosition({ 6.0f, -8.0f, -26.0f });
		mBuffer.push_back(mesh_buffer[9]);
		
		mBuffer[9].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[9].SetPosition({ 1.5f, 1.5f, 10.0f });
		mBuffer.push_back(mesh_buffer[10]);			 
		mBuffer[10].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[10].SetPosition({ -3.0f, 3.0f, 14.0f });
		mBuffer.push_back(mesh_buffer[11]);			 
		mBuffer[11].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[11].SetPosition({ 4.0f, -2.0f, 12.0f });
		mBuffer.push_back(mesh_buffer[12]);			
		mBuffer[12].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[12].SetPosition({ -6.0f, -6.0f, 20.0f });
		mBuffer.push_back(mesh_buffer[13]);			 
		mBuffer[13].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[13].SetPosition({ 7.0f, 6.0f, -23.0f });
		mBuffer.push_back(mesh_buffer[14]);
		mBuffer[14].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[14].SetPosition({ -10.0f, 8.0f, 25.0f });
		mBuffer.push_back(mesh_buffer[15]);
		mBuffer[15].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[15].SetPosition({ -10.0f, -8.0f, 25.0f });
		mBuffer.push_back(mesh_buffer[16]);
		mBuffer[16].SetScale({ 0.5f, 0.5f, 0.5f });
		mBuffer[16].SetPosition({ 6.0f, -8.0f, 26.0f });
	}
	
	for (auto& I : mBuffer)
		mListboxItem.push_back(I.GetObjectName().c_str());
	
	mFPSCamera = std::make_unique<FPSCamera>(Vector<float, 3>({ 0.0f, 0.0f, 0.0f }), Vector<float, 3>({0.0f, 0.0f, -1.0f}));
	mEditCamera = std::make_unique<EditCamera>(Vector<float, 3>({ 0.0f, 8.0f, 30.0f }), Vector<float, 3>({ 0.0f, 0.0f, 0.0f }));
	
	mLight = std::make_unique<Light>(Vector<float, 3>({ 0.0f, 12.0f, 0.0f }));

	GLcall(glEnable(GL_DEPTH_TEST));

	mShader = std::make_unique<Shader>("Color3D.shader");
	mShader->bind();

	m_u_Model = mShader->GetUniformLocation("u_Model");
	m_u_ViewProjection = mShader->GetUniformLocation("u_ViewProjection");
	m_u_LightColor = mShader->GetUniformLocation("u_LightColor");
	//m_u_LightPos = mShader->GetUniformLocation("u_LightPos");

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
			mEditCamera->SetDirection(mBuffer[current_object_id].GetPosition());

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
	mShader->SetUniform3f(m_u_LightColor, mLight->GetLightColor());
	//mShader->SetUniform3f(m_u_LightPos, mLight->GetLightPosition());

	for (auto& aux : mBuffer)
	{
		mModel = aux.GetScale() * aux.GetRotation() * aux.GetTranslation(); //Model view projection
		mViewProjection = mEditCamera->GetView() * mPersp;

		mShader->SetUniformMatrix4f(m_u_Model, mModel);
		mShader->SetUniformMatrix4f(m_u_ViewProjection, mViewProjection);

		aux.GetMesh()->GetVertexArray().bind();
		aux.GetMesh()->GetIndexBuffer().bind();
		mRenderer->Draw(aux.GetMesh()->GetIndexBuffer());
		aux.GetMesh()->GetVertexArray().unbind();
		aux.GetMesh()->GetIndexBuffer().unbind();
	}

	mShader->unbind();
}
