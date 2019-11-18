#include "Mesh.h"
#include "glad/glad.h"

Mesh::Mesh(std::vector<vertex> const& vertex_data, std::vector<unsigned int> const& indices)
	: m_vertex_data(vertex_data)
	, m_indices(indices)
{
	create_mesh();
}

Mesh::Mesh(std::vector<vertex>&& vertex_data, std::vector<unsigned int>&& indices)
	: m_vertex_data(vertex_data)
	, m_indices(indices)
{
	create_mesh();
}

Mesh::Mesh(Mesh const& other)
{
	copy_other(other);
}

Mesh& Mesh::operator=(Mesh const& other)
{
	if (this != &other)
		copy_other(other);
	return *this;
}

Mesh::Mesh(Mesh&& other)
{
	move_other(std::move(other));
}

Mesh& Mesh::operator=(Mesh&& other)
{
	if (this != &other)
		move_other(std::move(other));
	return *this;
}

Mesh::~Mesh()
{
}

void Mesh::copy_other(Mesh const& other)
{
	m_vertex_data = other.m_vertex_data;
	m_indices = other.m_indices;
	create_mesh();
}

void Mesh::move_other(Mesh&& other)
{
	m_vertex_data = std::move(other.m_vertex_data);
	m_indices = std::move(other.m_indices);
	create_mesh();
}

void Mesh::create_mesh()
{
	constexpr unsigned int NumAttrib = sizeof(vertex)/sizeof(float);
	m_vao.reset(new VertexArray());
	m_vb.reset(new VertexBuffer(reinterpret_cast<float *>(m_vertex_data.data()), m_vertex_data.size(), NumAttrib));
	m_ib.reset(new IndexBuffer(m_indices.data(), m_indices.size()));

	m_vao->PushLayout(3, GL_FLOAT, false, 0);  // Position
	m_vao->PushLayout(3, GL_FLOAT, false, 3);  // Normal
	m_vao->PushLayout(3, GL_FLOAT, false, 6);  // tangent
	m_vao->PushLayout(3, GL_FLOAT, false, 9);  // bitangent
	m_vao->PushLayout(2, GL_FLOAT, false, 12); // texcoord

	m_vao->AddBuffer(*m_vb);

	m_vao->unbind();
	m_vb->unbind();
	m_ib->unbind();
}
