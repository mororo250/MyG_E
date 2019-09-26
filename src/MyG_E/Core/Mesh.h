#pragma once

#include "Foundation/Math/Vector.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

enum class Shape
{
	PLANE,
	CUBE,
	SPHERE,
	PYRAMID
};

class Mesh
{
public:

	Mesh(Shape shape = Shape::CUBE);
	// copy It's a very expensive call should be avoid as much as possible.
	Mesh(Mesh const& other);
	Mesh& operator=(Mesh const& other);
	~Mesh();
	
	inline Shape get_shape() { return m_shape; }
	inline float* GetData() { return m_vertex_data.data(); }
	inline VertexArray const& GetVertexArray() const { return *m_vao; }
	inline IndexBuffer const& GetIndexBuffer() const { return *m_ib; }

private:
	void copy_other(Mesh const& other);

	void CreateMesh();

	static const std::vector<float> Mesh::plane_vertices;
	static const std::vector<unsigned int> Mesh::plane_indices;
	static const std::vector<float> cube_vertices;
	static const std::vector<unsigned int> cube_indices;
	static const std::vector<float> pyramid_vertices;
	static const std::vector<unsigned int> pyramid_indices;
	static const std::vector<float> sphere_vertices;
	static const std::vector<unsigned int> sphere_indices;

	Shape m_shape;
	std::vector<float> m_vertex_data;
	std::vector<unsigned int> m_indices;
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<IndexBuffer> m_ib;
	std::unique_ptr<VertexBuffer> m_vb;
};