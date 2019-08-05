#include "Texture2D.h"
#include "Core/BatchRenderer.h"
#include <filesystem>

Texture2D::Texture2D()
	:mTranX(384.0f), mTranY(512.0f), mScaleX(3.0f), mScaleY(3.0f), mAngle(0.0f),
	mScaleMat(1.0f, 1.0f),
	mRotMat(0.0f),
	mOrtho(CreateOrthoMatrix(0.0f, 1024, 768, 0)), 
	mTranMat(512.0f, 384.0f)
{
	float positions[] = {
	-50.0f, -50.0f, 0.0f, 0.0f, //index 0
	 50.0f, -50.0f, 1.0f, 0.0f, //index 1
	 50.0f, 50.0f, 1.0f, 1.0f , //index 2
	-50.0f, 50.0f, 0.0f, 1.0f  //index 3
	};
	

	unsigned int indices[] = {
	0, 1, 2, 2 ,3, 0
	};

	GLcall(glEnable(GL_BLEND));
	GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
	mVa = std::make_unique<VertexArray>();
	m_vb = std::make_unique<VertexBuffer>(positions, 4, 4);
	m_ib = std::make_unique<IndexBuffer>(indices, 6);

	mVa->PushLayout(2, GL_FLOAT, GL_FALSE, 0);
	mVa->PushLayout(2, GL_FLOAT, GL_FALSE, 2);
	mVa->AddBuffer(*m_vb);

	mShader = std::make_unique<Shader>("Shader.glsl");
	mShader->bind();

	Matrix<float, 3, 3> WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho; //Model view projection

	mU_MVP = mShader->GetUniformLocation("u_MVP");
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);

	std::string file_path = std::filesystem::current_path().parent_path().parent_path().parent_path().string();
	file_path += "\\src\\MyG_E\\Resources\\PS4.PNG";
	m_texture = std::make_unique<Texture>(file_path);

	m_texture->bind(0);
	mShader->SetUniform1i(mShader->GetUniformLocation("u_texture"), 0);

	mVa->unbind();
	m_vb->unbind();
	m_ib->unbind();
	mShader->unbind();

	m_renderer = std::make_unique<Renderer2D>();
}

Texture2D::~Texture2D()
{
	GLcall(glDisable(GL_BLEND));
}

void Texture2D::ImGuiRenderer()
{
	ImGui::SliderFloat("Translation X", &mTranX, 0.0f, 1024);
	ImGui::SliderFloat("Translation y", &mTranY, 0.0f, 768);
	ImGui::SliderFloat("Scale X", &mScaleX, 0.0f, 100.0f);
	ImGui::SliderFloat("Scale y", &mScaleY, 0.0f, 100.0f);
	ImGui::SliderFloat("Rotation", &mAngle, -6.28f, 6.28f);
}

void Texture2D::Update()
{
	m_renderer->Clear();

	mTranMat.SetTranX(mTranX);
	mTranMat.SetTranY(mTranY);
	mScaleMat.SetScaleX(mScaleX);
	mScaleMat.SetScaleY(mScaleY);
	mRotMat.SetAngle(mAngle);

	Matrix<float, 3, 3>  WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho;

	mShader->bind();
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);

	mVa->bind();
	m_ib->bind();
	m_renderer->Draw(*m_ib);
	mVa->unbind();
	m_ib->unbind();
	mShader->unbind();
}
