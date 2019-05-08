#include "Mesh.h"

Mesh::Mesh(const Vector<float, 4> &color)
{
	CreateVertexPosCube({ 1.0f, 1.0f, 1.0f });
	SetVertexColor(color);
	CreateCubeMesh();
}
Mesh::Mesh(const std::vector<Vector<float, 4>> &color)
{
	CreateVertexPosCube({ 1.0f, 1.0f, 1.0f });
	CreateVertexNormals();
	SetVertexColor(color);
	CreateCubeMesh();
}

Mesh::~Mesh()
{
}

void Mesh::CreateVertexPosCube(const Vector<float, 3> dimensions)
{
	mVertexData.reserve(24);
	VertexData3C aux;

	// Back
	// Botton-left-back
	aux.Position = { -dimensions[0], -dimensions[1], -dimensions[2] };
	mVertexData.push_back(aux);
	// Botton-right-back
	aux.Position = { dimensions[0], -dimensions[1], -dimensions[2] };
	mVertexData.push_back(aux);
	// Top-right-back
	aux.Position = { dimensions[0], dimensions[1], -dimensions[2] };
	mVertexData.push_back(aux);
	// Top-left-back
	aux.Position = { -dimensions[0], dimensions[1], -dimensions[2] };
	mVertexData.push_back(aux);

	// Front
	// Botton-left-front
	aux.Position = { -dimensions[0], -dimensions[1], dimensions[2] };
	mVertexData.push_back(aux);
	// Botton-right-front
	aux.Position = { dimensions[0], -dimensions[1], dimensions[2] };
	mVertexData.push_back(aux);
	// Top-right-front
	aux.Position = { dimensions[0], dimensions[1], dimensions[2] };
	mVertexData.push_back(aux);
	// Top-left-front
	aux.Position = { -dimensions[0], dimensions[1], dimensions[2] };
	mVertexData.push_back(aux);

	// Left
	// Left-botton-back
	mVertexData.push_back(mVertexData[0]);
	// Left-botton-front
	mVertexData.push_back(mVertexData[4]);
	// Left-top-front
	mVertexData.push_back(mVertexData[7]);
	// Left-top-back
	mVertexData.push_back(mVertexData[3]);

	//Right
	//Right-botton-back
	mVertexData.push_back(mVertexData[1]);
	//Right-botton-front
	mVertexData.push_back(mVertexData[5]);
	//Right-top-front
	mVertexData.push_back(mVertexData[6]);
	//Right-top-back
	mVertexData.push_back(mVertexData[2]);

	//Botton
	//Botton-left-back
	mVertexData.push_back(mVertexData[0]);
	//Botton-right-back
	mVertexData.push_back(mVertexData[1]);
	//Botton-right-front
	mVertexData.push_back(mVertexData[5]);
	//Botton-left-front
	mVertexData.push_back(mVertexData[4]);

	//Top
	//Top-left-back
	mVertexData.push_back(mVertexData[3]);
	//Top-right-back
	mVertexData.push_back(mVertexData[2]);
	//Top-right-front
	mVertexData.push_back(mVertexData[6]);
	//Top-left-fron
	mVertexData.push_back(mVertexData[7]);

	mIndices = {	// Back
					0,  1,  2,
					2,  3,  0,
					// Front
					4,  5,  6,
					6,  7,  4,
					// Left
					 8,  9, 10,
					10, 11,  8,
					// Right
					12, 13, 14,
					14, 15, 12,
					// Bottom
					16, 17, 18,
					18, 19, 16,
					// Top
					20, 21, 22,
					22, 23, 20 };
}

void Mesh::CreateVertexNormals()
{
	// Back 0 - 4
	for (unsigned int i = 0 ; i < 4; i++)
		mVertexData[i].Normals = { 0.0f, 0.0f, -1.0f };
	// Front 4 - 8
	for (unsigned int i = 4; i < 8; i++)
		mVertexData[i].Normals = { 0.0f, 0.0f, 1.0f };
	// Left 8 - 12
	for (unsigned int i = 8; i < 12; i++)
		mVertexData[i].Normals = { -1.0f, 0.0f, 0.0f };
	// Right 12 - 16
	for (unsigned int i = 12; i < 16; i++)
		mVertexData[i].Normals = { 1.0f, 0.0f, 0.0f };
	// Botton 16 - 20
	for (unsigned int i = 16; i < 20; i++)
		mVertexData[i].Normals = { 0.0f, -1.0f, 0.0f };
	// Top 20 - 24
	for (unsigned int i = 20; i < 24; i++)
		mVertexData[i].Normals = { 0.0f, 1.0f, 0.0f };
}

void Mesh::CreateCubeMesh()
{
	mVao = std::make_unique<VertexArray>();
	mVb = std::make_unique<VertexBuffer>(reinterpret_cast<float *>(mVertexData.data()), mVertexData.size(), 10);
	mIb = std::make_unique<IndexBuffer>(mIndices.data(), mIndices.size());

	mVao->PushLayout(3, GL_FLOAT, GL_FALSE, 0);
	mVao->PushLayout(3, GL_FLOAT, GL_FALSE, 3);
	mVao->PushLayout(4, GL_FLOAT, GL_FALSE, 6);

	mVao->AddBuffer(*mVb);

	mVao->unbind();
	mVb->unbind();
	mIb->unbind();
}

void Mesh::SetVertexColor(const std::vector<Vector<float, 4>>& color)
{
	for (unsigned int i = 0; i < mVertexData.size(); i++)
		mVertexData[i].Color = color[i];
}
void Mesh::SetVertexColor(const Vector<float, 4>& color)
{
	for (auto& i : mVertexData)
		i.Color = color;
}
