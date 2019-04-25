#pragma once

#include "Foundation/Math/Vector.h"

#include "Sprite.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


class Mesh
{
public:

	Mesh(const Vector<float, 4> &color = { 1.0f, 1.0f, 1.0f, 1.0f });
	Mesh(const std::vector<Vector<float, 4>> &color);
	
	~Mesh();
	
	inline VertexData3C* GetData() { return mVertexData.data(); }
	inline const VertexArray& GetVertexArray() { return *mVao; }
	inline const IndexBuffer& GetIndexBuffer() { return *mIb; }
private:

	std::vector<VertexData3C> mVertexData;
	std::vector<unsigned int> mIndices;
	std::unique_ptr<VertexArray> mVao;
	std::unique_ptr<IndexBuffer> mIb;
	std::unique_ptr<VertexBuffer> mVb;

	void CreateVertexPosCube(const Vector<float, 3> dimensions);
	void CreateCubeMesh();
	void SetVertexColor(const std::vector<Vector<float, 4>>& color);
	void SetVertexColor(const Vector<float, 4>& color);
};

