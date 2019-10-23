#pragma once

#include "Foundation/Math/Vector.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct vertex
{
	Vector3f position;
	Vector3f normal;
	Vector3f tangent;
	Vector3f bitagent;
	Vector2f textcoord;
};

class Mesh
{
public:
	Mesh(std::vector<vertex> const& vertex_data, std::vector<unsigned int> const& indices);
	Mesh(std::vector<vertex>&& vertex_data, std::vector<unsigned int>&& indices);

	// copy It's a very expensive call should be avoid as much as possible.
	Mesh(Mesh const& other);
	Mesh& operator=(Mesh const& other);

	// Move
	Mesh(Mesh&& other);
	Mesh& operator=(Mesh&& other);

	~Mesh();

	inline vertex* GetData() { return m_vertex_data.data(); }
	inline VertexArray const& GetVertexArray() const { return *m_vao; }
	inline IndexBuffer const& GetIndexBuffer() const { return *m_ib; }

private:
	void copy_other(Mesh const& other);
	void move_other(Mesh&& other);

	void CreateMesh();

	std::vector<vertex> m_vertex_data;
	std::vector<unsigned int> m_indices;
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<IndexBuffer> m_ib;
	std::unique_ptr<VertexBuffer> m_vb;
};
