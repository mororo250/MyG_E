#pragma once

#include "Foundation/Math/Vector.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

enum class Shape
{
	CUBE,
	SPHERE
};

class Mesh
{
public:

	Mesh(Shape shape = Shape::CUBE);
	~Mesh();
	
	inline float* GetData() { return mVertexData.data(); }
	inline const VertexArray& GetVertexArray() { return *mVao; }
	inline const IndexBuffer& GetIndexBuffer() { return *mIb; }
private:

	static const std::vector<float> cube_vertices;
	static const std::vector<unsigned int> cube_indices;
	static const std::vector<float> sphere_vertices;
	static const std::vector<unsigned int> sphere_indices;

	std::vector<float> mVertexData;
	std::vector<unsigned int> mIndices;
	std::unique_ptr<VertexArray> mVao;
	std::unique_ptr<IndexBuffer> mIb;
	std::unique_ptr<VertexBuffer> mVb;

	void CreateCubeMesh();
};