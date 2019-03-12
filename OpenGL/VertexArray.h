#pragma once

#include <vector>
#include "VertexBuffer.h"

struct VertexLayout
{
	unsigned int NumElem;
	unsigned int type;
	bool normalized;
	unsigned int offset;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb);
	void PushLayout(unsigned int NumElem, unsigned int type = GL_FLOAT, bool normalized = false, const unsigned int offset = 0);
	void bind() const;
	void unbind() const;

	inline void EnabelVertexArray() { GLcall(glEnableVertexAttribArray(mVertexArray)); }
	inline std::vector<VertexLayout> GetVertexLayouts() { return mLayoutVector; }
	inline unsigned int GetVertexArray() const { return mVertexArray; }
	

private:

	unsigned int mVertexArray; 
	std::vector<VertexLayout> mLayoutVector;
};
