#pragma once

#include "Foundation/Math/Vector.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

enum class Shape
{
	CUBE,
	SPHERE,
	PIRAMID
};

class Mesh
{
public:

	Mesh(Shape shape = Shape::CUBE);
	~Mesh();
	
	inline float* GetData() { return m_vertex_data.data(); }
	inline const VertexArray& GetVertexArray() { return *m_vao; }
	inline const IndexBuffer& GetIndexBuffer() { return *m_ib; }
private:

	static const std::vector<float> cube_vertices;
	static const std::vector<unsigned int> cube_indices;
	static const std::vector<float> sphere_vertices;
	static const std::vector<unsigned int> sphere_indices;
	static const std::vector<float> piramid_vertices;
	static const std::vector<unsigned int> piramid_indices;

	std::vector<float> m_vertex_data;
	std::vector<unsigned int> m_indices;
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<IndexBuffer> m_ib;
	std::unique_ptr<VertexBuffer> m_vb;

	void CreateMesh();
};